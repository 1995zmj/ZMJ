# 备忘录



### MonoBehaviour事件执行命令

由第一个到最后一个方法执行。

```csharp
private void Awake() { /* 加载脚本时调用 */ }
private void OnEnable() { /* 每次启用对象时调用 */ }
private void Start() { /* 启用脚本时在框架上调用 */ }
private void Update() { /* 每帧调用一次 */ }
private void LateUpdate() { /* 在更新后调用每一帧 */ }
private void OnBecameVisible() { /* 在渲染器可由任何Camera可见时调用 */ }
private void OnBecameInvisible() { /* 当渲染器不再由任何Camera可见时调用 */ }
private void OnDrawGizmos() { /* 允许您在“场景视图”中绘制Gizmos */ }
private void OnGUI() { /* 为响应GUI事件，每帧多次调用 */ }
private void OnApplicationPause() { /* 在检测到暂停时在帧末尾调用 */ }
private void OnDisable() { /* 每次禁用对象时调用 */ }
private void OnDestroy() { /* 仅在已被破坏的先前活动的GameObject上调用 */ }
```

Physics updates on a Fixed Timestep are defined under Edit ▸ Project Settings ▸ Time ▸ Fixed Timestep and may execute more or less than once per actual frame. 固定时间步长上的物理更新在编辑▸项目设置▸时间▸固定时间步长下定义，并且可能在每个实际帧中执行不止一次。

```csharp
private void FixedUpdate() { /* Called every Fixed Timestep */ }
```

### GameObject操作

```csharp
/* 创建一个GameObject */
Instantiate(GameObject prefab);
Instantiate(GameObject prefab, Transform parent);
Instantiate(GameObject prefab, Vector3 position, Quaternion rotation);
/* 实践 */
Instantiate(bullet);
Instantiate(bullet, bulletSpawn.transform);
Instantiate(bullet, Vector3.zero, Quaternion.identity);
Instantiate(bullet, new Vector3(0, 0, 10), bullet.transform.rotation);

/* 销毁GameObject */
Destroy(gameObject);

/* 查找GameObjects */
GameObject myObj = GameObject.Find("NAME IN HIERARCHY");
GameObject myObj = GameObject.FindWithTag("TAG");

/* 访问组件 */
Example myComponent = GetComponent<Example>();
AudioSource audioSource = GetComponent<AudioSource>();
Rigidbody rgbd = GetComponent<Rigidbody>();
```

### 矢量快速参考

X =左/右 Y =上/下 Z =前进/后退

```csharp
Vector3.right /* (1, 0, 0) */   Vector2.right /* (1, 0) */
Vector3.left /* (-1, 0, 0) */   Vector2.left /* (-1, 0) */
Vector3.up /* (0, 1, 0) */      Vector2.up /* (0, 1) */
Vector3.down /* (0, -1, 0) */   Vector2.down /* (0, -1) */
Vector3.forward /* (0, 0, 1) */
Vector3.back /* (0, 0, -1) */
Vector3.zero /* (0, 0, 0) */    Vector2.zero /* (0, 0) */
Vector3.one /* (1, 1, 1) */     Vector2.one /* (1, 1) */
float length = myVector.magnitude /* 此向量的长度 */
myVector.normalized /* 保持方向，但将长度减小为1 */
```

### 时间变量

```csharp
/* 自游戏开始以来的时间，以秒为单位 */
float timeSinceStartOfGame = Time.time;

/* The scale at which the time is passing */
float currentTimeScale = Time.timeScale;

/* Pause time */
Time.timeScale = 0;

/* 完成最后一帧所需的时间（以秒为单位） */
/* 与Update（）和LateUpdate（）一起使用 */
float timePassedSinceLastFrame = Time.deltaTime;

/* 执行物理和固定帧速率更新的时间间隔（以秒为单位） */
/* 与FixedUpdate（）一起使用 */
float physicsInterval =  Time.fixedDeltaTime;
```

### 物理事件

```csharp
/* 两个对象都必须具有对撞机，而一个对象必须具有刚体才能使这些事件正常工作 */
private void OnCollisionEnter(Collision hit) { Debug.Log(gameObject.name + " just hit " + hit.gameObject.name); }
private void OnCollisionStay(Collision hit) { Debug.Log(gameObject.name + " is hitting " + hit.gameObject.name); }
private void OnCollisionExit(Collision hit) { Debug.Log(gameObject.name + " stopped hitting " + hit.gameObject.name); }

/* 必须在碰撞器之一上检查触发器 */
private void OnTriggerEnter(Collider hit) { Debug.Log(gameObject.name + " just hit " + hit.name); }
private void OnTriggerStay(Collider hit) { Debug.Log(gameObject.name + " is hitting " + hit.name); }
private void OnTriggerExit(Collider hit) { Debug.Log(gameObject.name + " stopped hitting " + hit.name); }

/* 对于2D碰撞器，只需在方法名称和参数类型中添加2D */
private ​void OnCollisionEnter2D(Collision2D hit) { }
private void OnCollisionStay2D(Collision2D hit) { }
private void OnCollisionExit2D(Collision2D hit) { }
private void OnTriggerEnter2D(Collider2D hit) { }
private void OnTriggerStay2D(Collider2D hit) { }
private void OnTriggerExit2D(Collider2D hit) { }
```

### 协程

```csharp
/* 创建一个协程 */
private IEnumerator CountSeconds(int count = 10)
{
  for (int i = 0; i <= count; i++) {
    Debug.Log(i + " second(s) have passed");
    yield return new WaitForSeconds(1.0f);
  }
}

/* 调用一个协程 */
StartCoroutine(CountSeconds());
StartCoroutine(CountSeconds(10));

/* 调用可能需要停止的协程 */
StartCoroutine("CountSeconds");
StartCoroutine("CountSeconds", 10);

/* 停止协程 */
StopCoroutine("CountSeconds");
StopAllCoroutines();

/* 通过变量存储和调用协程 */
private IEnumerator countSecondsCoroutine;

countSecondsCoroutine = CountSeconds();
StartCoroutine(countSecondsCoroutine);

/* 停止存储的协程 */
StopCoroutine(countSecondsCoroutine);

/* 协程返回类型 */
yield ​return null; // 等待下一个Update（）调用
yield return new WaitForFixedUpdate(); // 等到下一个FixedUpdate（）调用
yield return new WaitForEndOfFrame(); // 等到一切该帧已经执行了
yield return new WaitForSeconds(float seconds); // 以秒为单位的游戏时间//等待
yield return new WaitUntil(() => MY_CONDITION); // 等待直到自定义条件被满足
yield return new WWW("MY/WEB/REQUEST"); // 等待Web请求
yield return StartCoroutine("MY_COROUTINE"); // 等待，直到另一个协程完成
```

### 输入快速参考

```csharp
if (Input.GetKeyDown(KeyCode.Space)) { Debug.Log("Space key was Pressed"); }
if (Input.GetKeyUp(KeyCode.W)) { Debug.Log("W key was Released"); }
if (Input.GetKey(KeyCode.UpArrow)) { Debug.Log("Up Arrow key is being held down"); }

/* Button Input located under Edit >> Project Settings >> Input */
if (Input.GetButtonDown("ButtonName")) { Debug.Log("Button was pressed"); }
if (Input.GetButtonUp("ButtonName")) { Debug.Log("Button was released"); }
if (Input.GetButton("ButtonName")) { Debug.Log("Button is being held down"); }
```

