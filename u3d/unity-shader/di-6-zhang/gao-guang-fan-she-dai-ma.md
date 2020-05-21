# 高光反射代码

```
// Upgrade NOTE: replaced '_Object2World' with 'unity_ObjectToWorld'

// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6-SpecularVertexLevel" 
{
    
    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
        _Specular("Specular", Color) = (1.0,1.0,1.0,1.0)
        _Gloss("Gloss", Range(8.0,256.0)) = 20
    }
    
    SubShader
    {
        Pass
        {
            //定义光照模式，只有正确定光照模式，才能得到一些Unity内置光照变量
            Tags{"LightMode" = "ForwardBase"}
        
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            //使用Unity的内置包含文件，使用其内置变量
            #include "Lighting.cginc"
        
            //定义与属性相同类型和相同名称的变量
            fixed4 _Diffuse;
            fixed4 _Specular;
            float _Gloss;
            //定义顶点着色器输入结构体
            struct a2v {
                float4 vertex:POSITION;
                float3 normal:NORMAL;
            };
            //定义顶点着色器输出结构体（片元着色器输入结构体）
            struct v2f {
                float4 pos:SV_POSITION;
                fixed3 color : COLOR;
            };
        
            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
                fixed3 worldNormal = UnityObjectToWorldNormal(v.normal);
                fixed3 worldLightDir = normalize(_WorldSpaceLightPos0.xyz);
                fixed3 diffuse = _LightColor0.rgb * _Diffuse.rgb* saturate(dot(worldNormal, worldLightDir));
                fixed3 reflectDir = normalize(reflect(-worldLightDir,worldNormal));
                fixed3 viewDir = normalize(_WorldSpaceCameraPos.xyz - mul(unity_ObjectToWorld,v.vertex).xyz);
                
                fixed3 specular = _LightColor0.rgb * _Specular.rgb * pow(saturate(dot(reflectDir,viewDir)),_Gloss);
                o.color = ambient + diffuse + specular;
                
                return o;
            }
            
            fixed4 frag(v2f i) :SV_Target{
                return fixed4(i.color,1.0);
            }
        
            ENDCG
        }
        
    }
    
    Fallback"Specular"
}  
```

```
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6-SpecularPixelLevel" 
{
    
    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
        _Specular("Specular", Color) = (1.0,1.0,1.0,1.0)
        _Gloss("Gloss", Range(8.0,256.0)) = 20.0
    }
    
    SubShader
    {
        Pass
        {
            //定义光照模式，只有正确定光照模式，才能得到一些Unity内置光照变量
            Tags{"LightMode" = "ForwardBase"}
        
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            //使用Unity的内置包含文件，使用其内置变量
            #include "Lighting.cginc"
        
            //定义与属性相同类型和相同名称的变量
            fixed4 _Diffuse;
            fixed4 _Specular;
            float _Gloss;
            //定义顶点着色器输入结构体
            struct a2v {
                float4 vertex:POSITION;
                float3 normal:NORMAL;
            };
            //定义顶点着色器输出结构体（片元着色器输入结构体）
            struct v2f {
                float4 pos:SV_POSITION;
                float3 worldNormal: TEXCOORD0;
                float3 viewDir : TEXCOORD1;
            };
        
            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                //通过模型到世界的转置逆矩阵计算得到世界空间内的顶点法向方向（v.normal存储的是模型空间内的顶点法线方向）
                 o.worldNormal = normalize(mul(v.normal,(float3x3)unity_WorldToObject));
	
                o.viewDir = normalize(_WorldSpaceCameraPos.xyz-mul(unity_ObjectToWorld,v.vertex).xyz);
                return o;
            }
            
            fixed4 frag(v2f i) :SV_Target{
                fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
                fixed3 worldLightDir = normalize(_WorldSpaceLightPos0.xyz);
                fixed3 worldNormal = i.worldNormal;
            
                fixed3 diffuse = _LightColor0.rgb*_Diffuse.rgb*saturate(dot(worldNormal, worldLightDir));
                fixed3 reflectDir = normalize(reflect(-worldLightDir, worldNormal));
                fixed3 viewDir = i.viewDir;
                fixed3 specular = _LightColor0.rgb * _Specular.rgb * pow(saturate(dot(viewDir, reflectDir)),_Gloss);
                fixed3 color = ambient + diffuse + specular;
                return fixed4(color,1.0);
            }
        
            ENDCG
        }
        
    }
    
    Fallback"Specular"
}  
```

```
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6-BlinnPhong" 
{
    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
        _Specular("Specular", Color) = (1.0,1.0,1.0,1.0)
        _Gloss("Gloss", Range(8.0,256.0)) = 20.0
    }
    
    SubShader
    {
        Pass
        {
            //定义光照模式，只有正确定光照模式，才能得到一些Unity内置光照变量
            Tags{"LightMode" = "ForwardBase"}
        
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            //使用Unity的内置包含文件，使用其内置变量
            #include "Lighting.cginc"
        
            //定义与属性相同类型和相同名称的变量
            fixed4 _Diffuse;
            fixed4 _Specular;
            float _Gloss;
            //定义顶点着色器输入结构体
            struct a2v {
                float4 vertex:POSITION;
                float3 normal:NORMAL;
            };
            //定义顶点着色器输出结构体（片元着色器输入结构体）
            struct v2f {
                float4 pos:SV_POSITION;
                float3 worldNormal: TEXCOORD0;
                float3 viewDir : TEXCOORD1;
            };
        
            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                //通过模型到世界的转置逆矩阵计算得到世界空间内的顶点法向方向（v.normal存储的是模型空间内的顶点法线方向）
                o.worldNormal = normalize(mul(v.normal,(float3x3)unity_WorldToObject));
	
                o.viewDir = normalize(_WorldSpaceCameraPos.xyz-mul(unity_ObjectToWorld,v.vertex).xyz);
                return o;
            }
            
            fixed4 frag(v2f i) :SV_Target{
                fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
                fixed3 worldLightDir = normalize(_WorldSpaceLightPos0.xyz);
                fixed3 worldNormal = i.worldNormal;
            
                fixed3 diffuse = _LightColor0.rgb*_Diffuse.rgb*saturate(dot(worldNormal, worldLightDir));
                fixed3 reflectDir = normalize(reflect(-worldLightDir, worldNormal));
                fixed3 viewDir = i.viewDir;
                fixed3 halfDir = normalize(worldLightDir + viewDir);
                fixed3 specular = _LightColor0.rgb * _Specular.rgb * pow(max(0, dot(worldNormal, halfDir)),_Gloss);
                fixed3 color = ambient + diffuse + specular;
                return fixed4(color,1.0);
            }
        
            ENDCG
        }
        
    }
    
    Fallback"Specular"
}  
```