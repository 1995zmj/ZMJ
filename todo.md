# ToDo

使用MaterialPropertyBlock来替换Material属性操作

public enum ShapeBehaviorType { Movement, Rotation, Oscillation }

public static class ShapeBehaviorTypeMethods { public static ShapeBehavior GetInstance\(this ShapeBehaviorType type\) { switch \(type\) { case ShapeBehaviorType.Movement: return ShapeBehaviorPool.Get\(\); case ShapeBehaviorType.Rotation: return ShapeBehaviorPool.Get\(\); case ShapeBehaviorType.Oscillation: return ShapeBehaviorPool.Get\(\); } UnityEngine.Debug.Log\("Forgot to support " + type\); return null; } }

扩展方法是静态类内部的静态方法，其行为类似于某种类型的实例方法。该类型可以是任何东西，类，接口，结构，原始值或枚举。扩展方法的第一个参数定义该方法将操作的类型和实例值。

这是否允许我们向所有内容添加方法？是的，就像您可以编写任何类型为参数的静态方法一样。这是一个好主意吗？当适度使用时，可以。它是一种有其用途的工具，但是如果遗弃它会产生非结构化的混乱。

sealed 修饰词 不让类被继承

平滑的过度 s =（3f - 2f  _s）_ s \* s;

隐形和显性转换 implicit implicit

编辑器相关 Editor folder.

属性的读写控制

通过将SelectionBase属性添加到，我们可以在场景窗口中强制选择内容根GameTileContent。

手动检查重叠目标，把要检测的统一管理 Collider\[\] targets = Physics.OverlapSphere\( transform.localPosition, targetingRange \);

渲染顺序

ugui 点的位置关系

Gizmos

Ray TouchRay =&gt; Camera.main.ScreenPointToRay\(Input.mousePosition\);

RequireComponent

return half4\(\_Color.r,\_Color.g,\_Color.b,color.a\);

