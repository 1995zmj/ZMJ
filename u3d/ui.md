# UI



{% embed url="https://zhuanlan.zhihu.com/pawniet" %}

坐标转换

* `Set Native Size` 作用
* 对于UI的图片把 `Texture Type` 设置成 `Sprite(2D and UI)`
* `Canvas Scaler(Script)` 画布缩放器，与适配相关
* ugui 点的位置关系

## 设置 RectTransform 大小

{% embed url="https://www.jianshu.com/p/4592bf809c8b" %}

[https://www.jianshu.com/p/dbefa746e50d](https://www.jianshu.com/p/dbefa746e50d)

```text
img.GetComponent<RectTransform>().SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 100);
```



