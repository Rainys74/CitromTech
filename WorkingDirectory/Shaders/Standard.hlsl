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

cbuffer CBuffer1
{
    matrix transform;
    float3 directionalLightDir;
    float3 cameraLocalPos;
};
cbuffer Material : register(b1)
{
    float u_Test;
    float4 u_ColorData;
};

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

float4 psmain(VSOut input) : SV_Target
{
    // Diffuse Lighting
    const float4 ambientColor = float4(0.42, 0.478, 0.627, 1.0);
    const float3 lightDiffuseColor = float3(1.0, 1.0, 1.0);
    const float3 materialDiffuseColor = float3(1.0, 1.0, 1.0);
    
    const float diffuseIntensity = 1.0;
    
    const float diffuseFactor = dot(normalize(input.normal), -directionalLightDir);
    
    float4 diffuseColor = float4(0, 0, 0, 0);
    
    if (diffuseFactor > 0)
    {
        diffuseColor = float4(lightDiffuseColor, 1.0f) * diffuseIntensity * float4(materialDiffuseColor, 1.0f) * diffuseFactor;
    }
    // ------------------------
    
    // Specular Lighting (Phong)
    const float3 materialSpecularColor = float3(1.0, 0.0, 0.0);
    // TODO: texture for specular exponent
    
    float4 specularColor = float4(0, 0, 0, 0);
    if (diffuseFactor > 0)
    {
        float3 pixelToCamera = normalize(cameraLocalPos - input.localPos);
        float3 lightReflect = normalize(reflect(directionalLightDir, input.normal));
        
        float specularFactor = dot(pixelToCamera, lightReflect);
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
    return tex.Sample(samplerTex, input.tex) * clamp(ambientColor + diffuseColor + specularColor, 0, 1); //(ambientColor + diffuseColor); // for diffuse only
}