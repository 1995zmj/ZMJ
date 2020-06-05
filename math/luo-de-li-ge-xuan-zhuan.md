# 罗德里格旋转

[http://zhaoxuhui.top/blog/2018/11/16/DerivationOfRodriguesRotationFormula.html](http://zhaoxuhui.top/blog/2018/11/16/DerivationOfRodriguesRotationFormula.html)

$$R = \cos\theta I + (1-\cos\theta)nn^T + \sin\theta n^\times$$

![](https://zhaoxuhui.top/assets/images/blog/content/2018-11-16-01.png)

矢量运算表达式

假设$$k$$为单位向量

$$v = v_\bot + v_\parallel$$

对于$$v_\parallel$$由于其大小可以看成是向量$$v$$在$$k$$上的投影，即$$v$$和$$k$$做点积，而其方向与$$k$$平行，可以表示成$$k$$的倍数，因此可以表示成如下形式：

$$v_\parallel = (k \cdot v)k$$

$$v_\bot = v - v\parallel = v - (k \cdot v)k$$

![](https://zhaoxuhui.top/assets/images/blog/content/2018-11-16-02.png)

连续的叉乘转化

$$v - v\parallel = v - (k \cdot v)k = -k \times(k \times v)$$

$$v_{\parallel rot} = v_\parallel$$

$$|v_{\bot rot}| = |v_\bot|$$

$$v_{\bot rot} = \cos\theta v_\bot + \sin\theta k \times v_\bot$$

因为$$k$$与$$v_\parallel$$平行，所以它们做叉乘结果为0向量，且根据$$v$$与投影分量的关系，有：

$$k \times v_\bot = k \times (v - v_\parallel) = k \times v - k \times v_\parallel = k \times v$$

$$v_{\bot rot} = \cos\theta v_\bot + \sin\theta k \times v$$

$$v_{rot} = v_{\parallel rot} + v_{\bot rot}$$

$$= v_\parallel + \cos\theta v_\bot + \sin\theta(k \times v)$$

$$= v_\parallel + \cos\theta (v - v_\parallel) + \sin\theta(k \times v)$$

$$= \cos\theta v + (1 - \cos\theta)v_\parallel + \sin\theta(k \times v)$$

$$= \cos\theta v + (1 - \cos\theta)(k \cdot v)k + \sin\theta(k \times v)$$



推导矩阵运算形式

$$K = k^\times$$

$$ 
K v = k \times v
$$

$$
K^2 v = k \times (k \times v)
$$

$$
v_\parallel = v + k \times(k \times v)
$$

$$
v_rot= \cos\theta v + (1 - \cos\theta)(v + k \times (k \times v)) + \sin\theta(k \times v)
$$

$$
v_rot= \cos\theta v + (1 - \cos\theta)v + (1 - \cos\theta)(k \times (k \times v)) + \sin\theta(k \times v)
$$

$$
v_rot= \cos\theta v + (1 - \cos\theta)v + (1 - \cos\theta)K^2 v+ \sin\theta Kv
$$

$$
v_rot= (\cos\theta  + (1 - \cos\theta) + (1 - \cos\theta)K^2 + \sin\theta K)v
$$

$$
R= \cos\theta  + (1 - \cos\theta) + (1 - \cos\theta)K^2 + \sin\theta K
$$

$$
K^2v = k \times (k \times v) = -(v - (k\cdot v)k) = (k \cdot v)k - v
$$