# 协程



### 等待时间

```csharp
using System.Collections;
using UnityEngine;

public class MyCoroutine : MonoBehaviour
{
    private int i;
    void Start()
    {
        i = 0;
        StartCoroutine(Thread1());
    }

    void Update()
    {
        
    }

    IEnumerator Thread1()
    {
        while(true){
            Debug.Log("i="+ i);
            i++;
            if(i >= 10)
            {
                break;
            }
            //等待一面
            yield return new WaitForSeconds(1.0f);
        }
    }
}
```



#### 等待其他协程结束

```csharp
using System.Collections;
using UnityEngine;

public class MyCoroutine : MonoBehaviour
{
    private int i;
    void Start()
    {
        i = 0;
        StartCoroutine(Thread1());
    }

    void Update()
    {
        
    }

    IEnumerator Thread1()
    {
        while(true){
            Debug.Log("i="+ i);
            i++;
            if(i >= 10)
            {
                break;
            }
            //等待一面
            yield return StartCoroutine(Thread2());
        }
    }

    IEnumerator Thread2()
    {
        Debug.Log("show idle");
        yield return null;
    }
}
```

