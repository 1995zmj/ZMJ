# 第5章



顶点/片元着色器基本结构

Unity Shader基本结构包含：Shader、Properties、SubShader、Fallback，即：

```text
Shader "MyShaderName" {

	Properties {
		// 属性
	}
    // 针对显卡 A 的 SubShader
	SubShader {
		Pass {
		
	    // 设置渲染状态和标签
		    
	    // 开始CG代码片段
			CGPROGRAM

      // 该代码片段的编译指令，例如
			#pragma vertex vert
			#pragma fragment frag
			
	    // CG代码写在这里

			ENDCG
			
			// 其他设置
		}
		// 其他需求的 Pass
	}
	
	SubShader {
        // 针对显卡 B 的 SubShader
	}
	
	// 上述 SubShader 都失败后用于回调的 Unity Shader
	Fallback "VertexLit"
}
```

比较重要的是Pass块内的内容

```text
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Unity Shaders Book/Chap er 5/Simple Shader"
{
    SubShader {
        Pass 
        {
            CGPROGRAM
            
            #pragma vertex vert
            #pragma fragment frag
            
            float4 vert(float4 v: POSITION) : SV_POSITION
            {
                return UnityObjectToClipPos(v);
            }
            
            float4 frag() : SV_Target
            {
                return fixed4(1.0,1.0,1.0,1.0);
            }
            
            ENDCG
        }
    }
}
```

由 **CGPROGRAM** 和 **ENDCG** 所包围的 CG 代码片段。

**vert** 函数的输入v包含顶点位置，通过**POSITION**语义指定

**POSITION** 将模型的顶点坐标填充到输入参数v中

**SV\_POSITION** 顶点着色器的输出是裁剪空间中顶点坐标

**SV\_Target** 将用户的输出颜色存储到一个渲染目标中，这里会输出到默认的帧缓存

**POSITION** 和 **SV\_POSITION** 均为CG/HLSL中的语义，语义告诉系统输入哪些值，输出哪些值，输出到哪里

**通过语义获得更多模型数据** **TEXCOORD0**：使用模型第一套纹理坐标填充texcoord变量，纹理坐标可以用来访问纹理**NORMAL**：获得法线方向，用于计算光照

```text
//通过一个结构体定义顶点着色器的输入
struct a2v {
    //使用POSITION语义，将模型空间的顶点坐标填充至vertex
    float4 vertex:POSITION;
    //使用NORMAL语义，将模型空间的顶点法线填充至normal（由于是矢量，这里使用float3）
    float3 normal:NORMAL;
    //使用TEXCOORD0语义，将模型的第一套纹理坐标填充texcoord变量
    float4 texcoord:TEXCOORD0;
};
```

填充到这些语义中的数据来自于模型的Mesh Render组件，每帧调用DrawCall时，Mesh Render组件会将负责渲染的模型的数据发给Unity。

a2v 的名字是什么意思呢？ a 表示应用 \(application\), v 表示顶点着色器 \(vertex shader\), a2v 的意思就是把数据 从应用阶段传递到顶点着色器中。

使用更加丰富的语义，获得模型更多信息：

```text
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter5_SimpleShader" 
{
    SubShader
    {
        Pass
        {
            CGPROGRAM 
            #pragma vertex vert
            #pragma fragment frag  
        
            //通过一个结构体定义顶点着色器的输入
            struct a2v {
                //使用POSITION语义，将模型空间的顶点坐标填充至vertex
                float4 vertex:POSITION;
                //使用NORMAL语义，将模型空间的顶点法线填充至normal（由于是矢量，这里使用float3）
                float3 normal:NORMAL;
                //使用TEXCOORD0语义，将模型的第一套纹理坐标填充texcoord变量
                float4 texcoord:TEXCOORD0;
            };
            
            //使用一个结构体定义片元着色器的输出
            struct v2f {
                //SV_POSITION语义告诉Unity，pos中包含模型顶点在裁剪空间的坐标
                float4 pos:SV_POSITION;
                //COLOR0语义告诉Unity,color用于存储颜色信息
                fixed3 color : COLOR0;
            };
        
            v2f vert(a2v v) {
                //声明输出结构
                v2f o;
                o.pos= UnityObjectToClipPos(v.vertex);
                //将法线方向映射到颜色中(法线矢量范围[-1,1],因此做一个映射计算)  
                o.color = v.normal*0.5 + fixed3(0.5,0.5,0.5);
                return o;
            }
        
            fixed4 frag(v2f i) : SV_Target{
                //将计算后的颜色显示出来
                return fixed4(i.color,1.0);
            }
    
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

**添加属性**

Shader中添加属性，可以在材质面板上看到对应的变量，并可以通过材质面板为该属性赋值。在计算输出时，需要在CG代码中，定义一个与属性名称和类型的相同的变量，才能在计算时使用该变量。

```text
Properties{
	_Color("Color Tint",Color) = (1.0,1.0,1.0,1.0)
}
```

CG代码片段中：

```text
// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'

Shader "Custom/Chapter5_SimpleShader" 
{
    Properties{
        _Color("Color Tint",Color) = (1.0,1.0,1.0,1.0)
    }
    
    SubShader
    {
        Pass
        {
            CGPROGRAM 
            #pragma vertex vert
            #pragma fragment frag  
        
            //通过一个结构体定义顶点着色器的输入
            struct a2v {
                //使用POSITION语义，将模型空间的顶点坐标填充至vertex
                float4 vertex:POSITION;
                //使用NORMAL语义，将模型空间的顶点法线填充至normal（由于是矢量，这里使用float3）
                float3 normal:NORMAL;
                //使用TEXCOORD0语义，将模型的第一套纹理坐标填充texcoord变量
                float4 texcoord:TEXCOORD0;
            };
            
            //使用一个结构体定义片元着色器的输出
            struct v2f {
                //SV_POSITION语义告诉Unity，pos中包含模型顶点在裁剪空间的坐标
                float4 pos:SV_POSITION;
                //COLOR0语义告诉Unity,color用于存储颜色信息
                fixed3 color : COLOR0;
            };
            
            float4 _Color;
        
            v2f vert(a2v v) {
                //声明输出结构
                v2f o;
                o.pos= UnityObjectToClipPos(v.vertex);
                //将法线方向映射到颜色中(法线矢量范围[-1,1],因此做一个映射计算)  
                o.color = v.normal*0.5 + fixed3(0.5,0.5,0.5);
                return o;
            }
        
            fixed4 frag(v2f i) : SV_Target{
                //将计算后的颜色显示出来
                fixed3 c = i.color;
                //使用_Color属性控制颜色属性
                c *= _Color.rgb;
                return fixed4(c,1.0);
            }
    
            ENDCG
        }
    }
    FallBack "Diffuse"
}
```

**Shaderlab属性类型和CG变量类型的匹配关系**

**Shaderlab属性类型**

Color, Vector

Range, Float

2D

Cube

3D

有时，读者可能会发现在 CG 变量前会有 uniform 关键字，例如： uniform fixed4 Color; unifom1 关键词是 CG 中修饰变量和参数的 种修饰词，它仅仅用于提供 些关于该变量的初 始值是如何指定和存储的相关信息（这和其他 些图像编程接口中的 uniform 关键词的作用不太 样）。在 Unity Shader 中， uniform 关键词是可以省略的。

Unity提供的内置文件和变量

unity 中一些常用的包含文件Editor\Data\CGIncludes

UnityCG.cginc包含了最常使用的帮助函数、 宏和结构体等

UnitySbader Variables.cginc包含了许多内置全局变量

调试

平台差异

整洁之道

1. 精度选择
2. 规范语法
3. 避免不必要的计算（shader target）
4. 慎用分支和循环语句
5. 不要除零

