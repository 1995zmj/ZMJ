# 漫反射代码

```text
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6_DiffuseVertexLevel" 
{

    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
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
                //通过模型到世界的转置逆矩阵计算得到世界空间内的顶点法向方向（v.normal存储的是模型空间内的顶点法线方向）
                fixed3 worldNormal = normalize(mul(v.normal,(float3x3)unity_WorldToObject));
                //得到世界空间内的光线方向
                fixed3 worldLight = normalize(_WorldSpaceLightPos0.xyz);

                //根据Lambert定律计算漫反射 saturate函数将所得矢量或标量的值限定在[0,1]之间
                fixed3 diffuse = _LightColor0.rgb*_Diffuse.rgb*saturate(dot(worldNormal,worldLight));

                o.color = diffuse + ambient;
                return o;
            }

            fixed4 frag(v2f i) :SV_Target{
                return fixed4(i.color,1.0);
            }

            ENDCG
        }

    }

    Fallback"Diffuse"
}
```

```text
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6-DiffusePixelLevel" 
{

    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
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
            //定义顶点着色器输入结构体
            struct a2v {
                float4 vertex:POSITION;
                float3 normal:NORMAL;
            };
            //定义顶点着色器输出结构体（片元着色器输入结构体）
            struct v2f {
                float4 pos:SV_POSITION;
                fixed3 worldNormal : TEXCOORD0;
            };

            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                //通过模型到世界的转置逆矩阵计算得到世界空间内的顶点法向方向（v.normal存储的是模型空间内的顶点法线方向）
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                return o;
            }

            fixed4 frag(v2f i) :SV_Target{
                fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
                fixed3 worldNormal = normalize(i.worldNormal);
                fixed3 worldLight = normalize(_WorldSpaceLightPos0.xyz);

                //根据Lambert定律计算漫反射 saturate函数将所得矢量或标量的值限定在[0,1]之间
                fixed3 diffuse = _LightColor0.rgb*_Diffuse.rgb*saturate(dot(worldNormal,worldLight));
                fixed3 color = ambient + diffuse;
                return fixed4(color,1.0);
            }

            ENDCG
        }

    }

    Fallback"Diffuse"
}
```

```text
// Upgrade NOTE: replaced '_World2Object' with 'unity_WorldToObject'
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter6-HalfLambert" 
{

    Properties
    {
        _Diffuse("DiffuseColor",Color) = (1.0,1.0,1.0,1.0)
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
            //定义顶点着色器输入结构体
            struct a2v {
                float4 vertex:POSITION;
                float3 normal:NORMAL;
            };
            //定义顶点着色器输出结构体（片元着色器输入结构体）
            struct v2f {
                float4 pos:SV_POSITION;
                fixed3 worldNormal : TEXCOORD0;
            };

            v2f vert(a2v v) {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                //通过模型到世界的转置逆矩阵计算得到世界空间内的顶点法向方向（v.normal存储的是模型空间内的顶点法线方向）
                o.worldNormal = UnityObjectToWorldNormal(v.normal);
                return o;
            }

            fixed4 frag(v2f i) :SV_Target{
                fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
                fixed3 worldNormal = normalize(i.worldNormal);
                fixed3 worldLight = normalize(_WorldSpaceLightPos0.xyz);

                //根据Lambert定律计算漫反射 saturate函数将所得矢量或标量的值限定在[0,1]之间
                fixed halfLambert = dot(worldNormal, worldLight) *0.5 + 0.5;
                fixed3 diffuse = _LightColor0.rgb*_Diffuse.rgb*halfLambert;
                fixed3 color = ambient + diffuse;
                return fixed4(color,1.0);
            }

            ENDCG
        }

    }

    Fallback"Diffuse"
}
```

