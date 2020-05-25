---
description: 贴图基础
---

# 第7章

通常使用一张纹理来代替物体的漫反射颜色。使用纹理的Shader中，需要对纹理进行采样。 使用CG的tex2D\(\_MainTex,uv\)函数进行纹理采样，第一个参数是需要被采样的纹理，第二是float2类型的纹理坐标，该坐标在顶点着色器中由\_MainTex\_ST对定点纹理坐标进行变换得到。

```text
Shader "Custom/Chapter7_SingleTexture" {
	Properties{
		_Color("Color",color) = (1,1,1,1)
		_MainTex("Main Tex", 2D) = "white"{}
		_Specular("Specular", color) = (1, 1, 1, 1)
		_Gloss("Gloss", Range(8.0, 256)) = 20
	}
		SubShader{
			Pass{
			Tags{"LightMode" = "ForwardBase"}

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "Lighting.cginc"

			fixed4 _Color;
			sampler2D _MainTex;
			//纹理类型的属性需要再声明一个float4类型的变量，命名格式为 纹理名_ST 
			//其中，ST是缩放和偏移的缩写 纹理名_ST.xy存储的是缩放值，纹理名_ST.zw存储的是偏移值
			//对应材质面板的纹理属性的Tiling和Offset调节项
			float4 _MainTex_ST;
			fixed4 _Specular;
			float   _Gloss;  

			struct a2v {
				float4 vertex:POSITION;
				float3 normal:NORMAL;

				//使用texcoord变量存储模型的第一组纹理坐标
				float4 texcoord:TEXCOORD0;  
			};

			struct v2f {
				float4 pos:SV_POSITION;
				float3 worldNormal:TEXCOORD0;
				float3 worldPos:TEXCOORD1;
				float2 uv:TEXCOORD2;

			};

			v2f vert(a2v v) {
				v2f o;
				o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				o.worldNormal = UnityObjectToWorldNormal(v.normal);
				o.worldPos = mul(_Object2World, v.vertex).xyz;
				//对纹理坐标进行变换，对应材质面板的Tiling和Offset调节项
				o.uv = v.texcoord.xy*_MainTex_ST.xy + _MainTex_ST.zw;
				//也可以使用内置函数  o.uv=TRANSFORM_TEX(v.texcoord,_MainTex); 计算过程一样

				return o;
			}
			fixed4 frag(v2f i) :SV_Target{
				fixed3 worldNormal = normalize(i.worldNormal);
				fixed3 worldLightDir = normalize(UnityWorldSpaceLightDir(i.worldPos));

				//使用tex2D做纹理采样，将采样结果和颜色属性_Color相乘作为反射率
				fixed3 albedo = tex2D(_MainTex,i.uv).rgb*_Color.rgb;

				fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz*albedo;

				fixed3 diffuse = _LightColor0.rgb*albedo*max(0,dot(worldNormal,worldLightDir));

				fixed3 viewDir = normalize(UnityWorldSpaceViewDir(i.worldPos));
				fixed3 halfDir = normalize(worldLightDir+viewDir);
				fixed3 specular = _LightColor0.rgb*albedo*pow(saturate(dot(halfDir,worldNormal)), _Gloss);

				return fixed4(ambient+diffuse+specular,1.0);
			}
			ENDCG		
	}
	}

	Fallback "Specular"
}
```



### 凹凸映射

凹凸映射的目的是使用一张纹理来修改模型表面的法线。这种方法不会真正该改变模型顶点位置，使模型看起来具有凹凸的效果，这点从模型的轮廓可以看出来。  
凹凸映射的两种方法：

* **高度纹理** 使用一张高度纹理来模拟表面位移，得到一个修改后的法线值，该方法也叫“高度映射"。 高度图中存储的是强度值，用于表示模型表面的海拔高度。颜色越浅表示越向外凸起，颜色越深越向里凹。实时计算中，不能直接得到表面法线，需要由像素的灰度值计算得到，需要消耗更多性能。
* **法线纹理** 使用一张法线纹理直接存储表面法线，该方法也叫“法线映射”。 法线纹理中存储的是表面的法线方向，法线矢量值范围\[-1,1\]，纹理中的颜色值范围\[0,1\]，因此将法线存储在一张纹理中，有一个映射过程： **pixel= \( normal+1\)/2** 因此在对法线纹理进行采样得到像素颜色后，为了得到对应的发现方向，需要进行反映射过程 **normal=2\(pixel\)-1**

**法线方向所在的坐标空间**

* 模型空间的法线纹理 模型顶点自带的法线，定义在模型空间中，若将修改后的模型空间中的表面法线存储在一张纹理中，则为模型空间中的法线纹理
* 切线空间的法线纹理

