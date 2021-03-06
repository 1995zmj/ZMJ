# RectMask2D

> RectMask2D不需要依赖一个Image组件，其裁剪区域就是它的RectTransform的rect大小。  
> **性质1：RectMask2D节点下的所有孩子都不能与外界UI节点合批且多个RectMask2D之间不能合批。【亲测不能合批】**  
> **性质2：计算depth的时候，所有的RectMask2D都按一般UI节点看待，只是它没有CanvasRenderer组件，不能看做任何UI控件的bottomUI。**

# Mask

> Mask组件需要依赖一个Image组件，裁剪区域就是Image的大小。  
> **性质1：Mask会在首尾（首=Mask节点，尾=Mask节点下的孩子遍历完后）多出两个drawcall，多个Mask间如果符合合批条件这两个drawcall可以对应合批（mask1 的首 和 mask2 的首合；mask1 的尾 和 mask2 的尾合。首尾不能合）**  
> **性质2：计算depth的时候，当遍历到一个Mask的首，把它当做一个不可合批的UI节点看待，但注意可以作为其孩子UI节点的bottomUI。**  
> **性质3：Mask内的UI节点和非Mask外的UI节点不能合批，但多个Mask内的UI节点间如果符合合批条件，可以合批。**

从Mask的性质3可以看出，并不是Mask越多越不好，因为Mask间是可以合批的。得出以下结论：  
**当一个界面只有一个mask，那么，RectMask2D 优于 Mask**  
**当有两个mask，那么，两者差不多。**  
**当 大于两个mask，那么，Mask 优于 RectMask2D。**