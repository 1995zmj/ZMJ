# Moller Trumbore
[参考](https://www.blurredcode.com/2020/04/%E7%9B%B4%E7%BA%BF%E4%B8%8E%E4%B8%89%E8%A7%92%E5%BD%A2%E7%9B%B8%E4%BA%A4moller-trumbore%E7%AE%97%E6%B3%95%E6%8E%A8%E5%AF%BC/)
Moller Trumbore算法是一种快速求解直线与三角形求交的算法，通过向量与矩阵计算可以快速得出交点与重心坐标。要推导它还比较麻烦，需要用到向量的[混合积](../数学/线性代数/三矢量的混合积.md)，[重心坐标](重心坐标.md)和[克拉莫法则](../数学/线性代数/克拉莫法则.md)。

已知光线 $Ray = O + t\vec{D}$，三角形三个顶点$P_0,P_1,P_2$。光线与三角形相交时，有如下等式：

$$Ray = O + t\vec{D} = (1-b_1-b_2)P_0 + b_1P_1 + b_2P_2$$

$$\to O - P_0 = (P_1 - P0)b_1 + (P_2 - P_0)b_2 - t\vec{D}$$

观察一下上面的括号以及等式左边的内容，都是已知的点，因此点的加减可以用向量来表示：

$$\vec{E_1} = P_1 - P_0$$

$$\vec{E_2} = P_2 - P_0$$

$$\vec{S} = O - P_0$$

得到

$$\vec{S} =\vec{E_1}b_1 + \vec{E_2}b_2 - t\vec{D}$$

$$\to \begin{bmatrix} -\vec{D}&\vec{E_1}&\vec{E_2} \end{bmatrix} \begin{bmatrix} t\\\ b1\\\ b2\end{bmatrix} = \vec{S}$$

这是一个形如 $Ax=c$的等式，所以可以用克拉莫法则。
 
$$t = \frac{\det\begin{pmatrix}\begin{bmatrix} \vec{S}&\vec{E_1}&\vec{E_2}\end{bmatrix}\end{pmatrix}}{\det\begin{pmatrix}\begin{bmatrix} -\vec{D}&\vec{E_1}&\vec{E_2}\end{bmatrix}\end{pmatrix}}$$

由向量混合积可以得出分母部分

$$\det\begin{pmatrix}\begin{bmatrix} -\vec{D}&\vec{E_1}&\vec{E_2}\end{bmatrix}\end{pmatrix} = -\vec{D} \cdot \vec{E_1} \times \vec{E_2} =  \vec{E_1}\cdot \vec{E_2} \times (-\vec{D}) = \vec{E_1}\cdot \vec{D} \times \vec{E_2}$$
   
$$\det\begin{pmatrix}\begin{bmatrix} \vec{S}&\vec{E_1}&\vec{E_2}\end{bmatrix}\end{pmatrix} = \vec{S} \times \vec{E_1} \cdot \vec{E_2}$$

同理可以解得剩下的两个未知变量，整理公式最终得到

$$\vec{E_1} = P_1 - P_0$$

$$\vec{E_2} = P_2 - P_0$$

$$\vec{S} = O - P_0$$

$$\vec{S_1} = \vec{D} \times \vec{E_2}$$

$$\vec{S_2} = \vec{S} \times \vec{E_1}$$

$$\begin{bmatrix} t\\\ b1\\\ b2\end{bmatrix} = \frac{1}{\vec{S_1} \cdot \vec{E_1}} \begin{bmatrix} \vec{S_2} \cdot \vec{E_2}\\\ \vec{S_1} \cdot \vec{S}\\\ \vec{S_2} \cdot \vec{D} \end{bmatrix} $$









