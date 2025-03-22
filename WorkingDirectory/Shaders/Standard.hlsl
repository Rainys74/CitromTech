/*cbuffer CBuf
{
    matrix transform;
};

float4 vsmain(float3 pos : Position) : SV_Position
{
    //float4 transformedPosition = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    //return float4(transformedPosition.x, transformedPosition.y, 0.0f, 1.0f);
    return float4(pos.x, pos.y, 0.0f, 1.0f);
}

float4 psmain() : SV_Target
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f);
}*/

struct VSInput
{
    float3 pos : Position;
    float3 n : Normal;
    float2 tex : TexCoord;
};
struct VSOut
{
    float2 tex : TexCoord;
    float4 pos : SV_Position;
    float3 normal : Normal;
    
    float3 localPos : LocalPosition;
};

#include <ShaderInterop/Matrices_ShaderInterop.h>
#include <ShaderInterop/Lighting_ShaderInterop.h>
#include <ShaderInterop/StandardMaterial_ShaderInterop.h>

VSOut vsmain(VSInput input)
{
    VSOut vso;
    vso.pos = mul(float4(input.pos, 1.0f), transform); //mul(mul(float4(input.pos, 1.0f), viewProjection), modelMatrix); //mul(mul(float4(input.pos, 1.0f), modelMatrix), viewProjection);
    vso.normal = input.n;
    vso.tex = input.tex;
    
    vso.localPos = input.pos;
    return vso;
}

Texture2D tex;
SamplerState samplerTex;

const float Lighting_GetDiffuseFactor(const float3 normal, const float3 lightDir)
{
    return dot(normalize(normal), -lightDir); // should i max with 0.0f?
}
const float3 Lighting_GetViewDirection(const float3 vertexLocalPos) // Pixel To Camera
{
    return normalize(cameraLocalPos - vertexLocalPos);
}

float4 Lambertian_DiffuseLighting(float3 normal, float3 lightDir, float3 lightColor, float lightIntensity, float3 albedo, float metallic)
{
    const float diffuseFactor = Lighting_GetDiffuseFactor(normal, lightDir);
    
    float4 diffuseColor = float4(0, 0, 0, 0);
    
    const float3 materialDiffuseColor = albedo * (1.0 - metallic); // Diffuse exists only for non-metallic surfaces
    
    if (diffuseFactor > 0)
    {
        diffuseColor = float4(lightColor, 1.0f) * lightIntensity * float4(materialDiffuseColor, 1.0f) * diffuseFactor;
    }
    
    return diffuseColor;
}
float4 Burley_DiffuseLighting(float3 normal, float3 lightDir, float3 viewDir, float3 lightColor, float lightIntensity, float3 albedo, float metallic, float roughness) // TODO: test
{
    const float NoL = Lighting_GetDiffuseFactor(normal, lightDir);

    #ifndef ADVANCED_BURLEY
    // Burley Diffuse Model
    float fd90 = 0.5 + 2.0 * NoL * NoL * roughness; // Fresnel term for grazing angles
    float lightScatter = 1.0 + (fd90 - 1.0) * pow(1.0 - NoL, 5.0); // Burley function

    // Apply metallic workflow (Metals don't have diffuse)
    float3 diffuseColor = albedo * (1.0 - metallic) * lightColor * lightIntensity * NoL * lightScatter;

    return float4(diffuseColor, 1.0);
    #else // TODO: does this even work??
    // Dot products
    float NoV = max(dot(normal, viewDir), 0.0);  // Normal * View
    float NoL = Lighting_GetDiffuseFactor(normal, lightDir); // Normal * Light

    // Burley Diffuse Model (Disney)
    float roughnessSquared = roughness * roughness;
    
    // Fresnel term for grazing angles
    float fd90 = 0.5 + 2.0 * roughnessSquared * NoL * NoL;

    // Schlick-like Fresnel approximation for diffuse
    float lightScatter = 1.0 + (fd90 - 1.0) * pow(1.0 - NoL, 5.0);
    float viewScatter = 1.0 + (fd90 - 1.0) * pow(1.0 - NoV, 5.0);

    // Final Burley Diffuse Contribution
    float diffuseFactor = NoL * lightScatter * viewScatter;

    // Apply metallic workflow (metals don't have diffuse)
    float3 diffuseColor = albedo * (1.0 - metallic) * lightColor * lightIntensity * diffuseFactor;

    return float4(diffuseColor, 1.0);
    #endif
}

float Phong_GetSpecularFactor(const float3 normal, const float3 localVertPos, const float3 lightDir)
{
    const float3 pixelToCamera = Lighting_GetViewDirection(localVertPos);
    const float3 lightReflect = normalize(reflect(lightDir, normal)); // reflectionDirection // do i need to reverse lightdir?
        
    return dot(pixelToCamera, lightReflect);
}
float BlinnPhong_GetSpecularFactor(const float3 normal, const float3 localVertPos, const float3 lightDir)
{
    const float3 pixelToCamera = Lighting_GetViewDirection(localVertPos);
    const float3 halfwayVec = normalize(pixelToCamera + -lightDir); // blinn-phong // (viewDirection + lightDirection)
        
    return dot(normal, halfwayVec); // blinn-phong, Phong: dot(pixelToCamera, lightReflect);
}

float4 PhongShared_SpecularLighting(float specularFactor, float3 normal, float3 localVertPos, float3 lightDir, float3 lightColor, float3 albedo, float metallic, float smoothness) // PhongShared/SharedPhong
{
    static const float3 F0_Dielectric = float3(0.04f, 0.04f, 0.04f); // Dielectric reflectance (fixed 0.04 for non-metals)
    
    const float3 materialSpecularColor = lerp(F0_Dielectric, mat_Albedo, mat_Metallic); // If metallic, specular is baseColor, else it's 0.04
    // TODO: texture for specular exponent
    
    float4 specularColor = float4(0, 0, 0, 0);
    
    if (Lighting_GetDiffuseFactor(normal, lightDir) > 0)
    {
        if (specularFactor > 0)
        {
            //float specularExponent = tex.Sample(samplerTex, input.tex).r * 255.0;
            float specularExponent = smoothness * 255.0; // 1.0 can stands for smoothness (like unity's pre-pbr model and stuff) however look into replacing with roughness to fit into pbr easier
            specularFactor = pow(abs(specularFactor), specularExponent);
            specularColor = float4(lightColor, 1.0f) * float4(materialSpecularColor, 1.0f) * specularFactor;
        }
    }
    
    return specularColor;
}
float4 BlinnPhong_SpecularLighting(float3 normal, float3 localVertPos, float3 lightDir, float3 lightColor, float3 albedo, float metallic, float smoothness)
{
    return PhongShared_SpecularLighting(BlinnPhong_GetSpecularFactor(normal, localVertPos, lightDir), normal, localVertPos, lightDir, lightColor, albedo, metallic, smoothness);
}
float4 Phong_SpecularLighting(float3 normal, float3 localVertPos, float3 lightDir, float3 lightColor, float3 albedo, float metallic, float smoothness)
{
    return PhongShared_SpecularLighting(Phong_GetSpecularFactor(normal, localVertPos, lightDir), normal, localVertPos, lightDir, lightColor, albedo, metallic, smoothness);
}

float4 psmain(VSOut input) : SV_Target
{
    static const float3 F0_Dielectric = float3(0.04f, 0.04f, 0.04f); // Dielectric reflectance (fixed 0.04 for non-metals)
    
    // Diffuse Lighting (Lambertian Diffuse)
    //const float4 ambientColor = float4(0.42, 0.478, 0.627, 1.0);
    //const float3 lightDiffuseColor = float3(1.0, 1.0, 1.0);
    const float3 lightDiffuseColor = float3(directionalLights[0].base.color);
    const float3 directionalLightDir = directionalLights[0].direction; // TODO: temporary   
    const float diffuseIntensity = directionalLights[0].base.color.a; //1.0; // TODO: you're probably gonna want to switch to albedo
    
    const float4 diffuseColor = Lambertian_DiffuseLighting(input.normal, directionalLightDir, lightDiffuseColor, diffuseIntensity, mat_Albedo, mat_Metallic);
    //float4 diffuseColor = Burley_DiffuseLighting(input.normal, directionalLightDir, Lighting_GetViewDirection(input.localPos), lightDiffuseColor, diffuseIntensity, mat_Albedo, mat_Metallic, mat_Roughness);
    // ------------------------
    
    // Specular Lighting (Blinn-Phong)
    //const float3 materialSpecularColor = float3(1.0, 0.0, 0.0);
    /*const float3 materialSpecularColor = lerp(F0_Dielectric, mat_Albedo, mat_Metallic); // If metallic, specular is baseColor, else it's 0.04
    // TODO: texture for specular exponent
    
    float4 specularColor = float4(0, 0, 0, 0);
    if (Lighting_GetDiffuseFactor(input.normal, directionalLightDir) > 0)
    {
        float3 pixelToCamera = Lighting_GetViewDirection(input.localPos);
        float3 lightReflect = normalize(reflect(directionalLightDir, input.normal)); // reflectionDirection // do i need to reverse lightdir?
        
        float3 halfwayVec = normalize(pixelToCamera + -directionalLightDir); // blinn-phong // (viewDirection + lightDirection)
        
        float specularFactor = dot(input.normal, halfwayVec); // blinn-phong, Phong: dot(pixelToCamera, lightReflect);
        if (specularFactor > 0)
        {
            //float specularExponent = tex.Sample(samplerTex, input.tex).r * 255.0;
            float specularExponent = (1.0 - mat_Roughness) * 255.0; // 1.0 can stands for smoothness (like unity's pre-pbr model and stuff) however look into replacing with roughness to fit into pbr easier
            specularFactor = pow(abs(specularFactor), specularExponent);
            specularColor = float4(lightDiffuseColor, 1.0f) * float4(materialSpecularColor, 1.0f) * specularFactor;
        }
    }*/
    const float4 specularColor = BlinnPhong_SpecularLighting(input.normal, input.localPos, directionalLightDir, lightDiffuseColor, mat_Albedo, mat_Metallic, (1.0 - mat_Roughness));
    // -------------------------
    
    ///return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    //return tex.Sample(samplerTex, input.tex);
    ///return u_ColorData;
    return tex.Sample(samplerTex, input.tex) * (float4(skyLight.base.color, 1.0f) + diffuseColor + specularColor); // clamp(x, 0, 1); ? //(ambientColor + diffuseColor); // for diffuse only
}