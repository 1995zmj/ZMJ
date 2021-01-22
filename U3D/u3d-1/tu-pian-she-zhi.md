# 图片设置

#### Wrap Mode

1. **Repeat**如果纹理坐标超过了 1，那么它的整数部分将会被舍弃，而直接使用小数部分进行采样，这样的结果是纹理将会不断重复。
2. **Clamp**如果纹理坐标大于1，那么将会截取到1， 如果小于O， 那么将会截取0 。

#### mipmapping

多级渐远纹理

首先将纹理类型（Texture Type）选择成Advanced，再勾选GenerateMipMaps即可开启多级渐远纹理技术。

