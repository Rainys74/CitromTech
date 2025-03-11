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

/*float4 BlinnPhong_SpecularLighting(float3 localVertPos, float3 normal) // TODO: Phong and PhongShared/SharedPhong
{
    float4 specularColor = float4(0, 0, 0, 0);
    
    float3 pixelToCamera = normalize(cameraLocalPos - localVertPos);
    float3 lightReflect = normalize(reflect(directionalLightDir, normal)); // reflectionDirection // do i need to reverse lightdir?
        
    float3 halfwayVec = normalize(pixelToCamera + -directionalLightDir); // blinn-phong // (viewDirection + lightDirection)
        
    float specularFactor = dot(normal, halfwayVec); // blinn-phong, Phong: dot(pixelToCamera, lightReflect);
    if (specularFactor > 0)
    {
            //float specularExponent = tex.Sample(samplerTex, input.tex).r * 255.0;
        float specularExponent = 1.0 * 255.0; // 1.0 can stands for smoothness (like unity's pre-pbr model and stuff) however look into replacing with roughness to fit into pbr easier
        specularFactor = pow(abs(specularFactor), specularExponent);
        specularColor = float4(lightDiffuseColor, 1.0f) * float4(materialSpecularColor, 1.0f) * specularFactor;
    }
    
    return specularColor;
}*/

float4 psmain(VSOut input) : SV_Target
{
    static const float3 F0_Dielectric = float3(0.04f, 0.04f, 0.04f); // Dielectric reflectance (fixed 0.04 for non-metals)
    
    // Diffuse Lighting (Lambertian Diffuse)
    //const float4 ambientColor = float4(0.42, 0.478, 0.627, 1.0);
    //const float3 lightDiffuseColor = float3(1.0, 1.0, 1.0);
    const float3 lightDiffuseColor = directionalLights[0].base.color;
    const float3 materialDiffuseColor = mat_Albedo * (1.0 - mat_Metallic); // Diffuse exists only for non-metallic surfaces
    
    const float3 directionalLightDir = directionalLights[0].direction; // TODO: temporary
    
    const float diffuseIntensity = directionalLights[0].base.intensity; //1.0; // TODO: you're probably gonna want to switch to albedo
    
    const float diffuseFactor = dot(normalize(input.normal), -directionalLightDir); // should i max with 0.0f?
    
    float4 diffuseColor = float4(0, 0, 0, 0);
    
    if (diffuseFactor > 0)
    {
        diffuseColor = float4(lightDiffuseColor, 1.0f) * diffuseIntensity * float4(materialDiffuseColor, 1.0f) * diffuseFactor;
    }
    // ------------------------
    
    // Specular Lighting (Blinn-Phong)
    //const float3 materialSpecularColor = float3(1.0, 0.0, 0.0);
    const float3 materialSpecularColor = lerp(F0_Dielectric, mat_Albedo, mat_Metallic); // If metallic, specular is baseColor, else it's 0.04
    // TODO: texture for specular exponent
    
    float4 specularColor = float4(0, 0, 0, 0);
    if (diffuseFactor > 0)
    {
        float3 pixelToCamera = normalize(cameraLocalPos - input.localPos);
        float3 lightReflect = normalize(reflect(directionalLightDir, input.normal)); // reflectionDirection // do i need to reverse lightdir?
        
        float3 halfwayVec = normalize(pixelToCamera + -directionalLightDir); // blinn-phong // (viewDirection + lightDirection)
        
        float specularFactor = dot(input.normal, halfwayVec); // blinn-phong, Phong: dot(pixelToCamera, lightReflect);
        if (specularFactor > 0)
        {
            //float specularExponent = tex.Sample(samplerTex, input.tex).r * 255.0;
            float specularExponent = 1.0 * 255.0; // 1.0 can stands for smoothness (like unity's pre-pbr model and stuff) however look into replacing with roughness to fit into pbr easier
            specularFactor = pow(abs(specularFactor), specularExponent);
            specularColor = float4(lightDiffuseColor, 1.0f) * float4(materialSpecularColor, 1.0f) * specularFactor;
        }
    }
    // -------------------------
    
    ///return float4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    //return tex.Sample(samplerTex, input.tex);
    ///return u_ColorData;
    return tex.Sample(samplerTex, input.tex) * clamp(float4(skyLight.base.color, 1.0f) + diffuseColor + specularColor, 0, 1); //(ambientColor + diffuseColor); // for diffuse only
}