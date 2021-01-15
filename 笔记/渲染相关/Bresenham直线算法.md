# Bresenham直线算法（布雷森汉姆算法）
[参考](https://oldj.net/blog/2010/08/27/bresenham-algorithm)
[视频教程](https://www.bilibili.com/video/BV1eE411p7tn?from=search&seid=3607722154107288400)

## 解决问题

* 在平面直接坐标系中，整数坐标点上绘制直线

## 优点

* 高效整数运算只用加减
* 不会产生断点

## 推导

### 先考虑简单情况，直线斜率大于0，小于等于1

![](QQ20210114191153.png)

```C#
 void draw(int x1,int y1,int x2, int y2)
{
	float k = (y2 - y1) * 1.0f  / (x2 - x1);

	float deltaY = 0;
	float middle = 0.5f;

	int y = y1;

	for (int x = x1; x <= x2; x++)
	{
		if (deltaY >= middle)
		{
			y += 1;
			middle += 1;
		}

		addPrefab(x, y);
		deltaY += k;
	}
}

```

 问题
* 只能在1/8圆的角度画直线
* 使用了浮点数

### 去除小数

同比例放大，都乘以一个2dx
乘法只用乘2操作，可用位移运算代替

![](QQ20210114194411.png)
```c#
void draw(int x1,int y1,int x2, int y2)
{
	int dx = Mathf.Abs(x2 - x1);
	int dy = Mathf.Abs(y2 - y1);

	float d2y = dy << 1;
	float d2x = dx << 1;

	float deltaY = d2y;
	float middle = dx;

	bool interchange = false;

	addPrefab(x1, y1);
	int y = y1;
	for (int x = x1 + 1; x < x2; x++)
	{
		if (deltaY >= middle)
		{
			y += 1;
			middle += d2x;
		}

		addPrefab(x, y);
		deltaY += d2y;
	}
	addPrefab(x2, y2);
}
```

### 全角度
当终点坐标比七点坐标小时，更改增长符号

当dx是短轴时，交换横纵轴，使dx永远指向长轴

```c#
void draw(int x1,int y1,int x2, int y2)
{
	int dx = Mathf.Abs(x2 - x1);
	int dy = Mathf.Abs(y2 - y1);

	int dirX = (x2 - x1) > 0 ? 1 : -1;
	int dirY = (y2 - y1) > 0 ? 1 : -1;


	int d2y = dy << 1;
	int d2x = dx << 1;

	int deltaY = d2y;
	int addMiddle = d2x;
	int addDeltaY = d2y;
	int middle = dx;
	int addX = dirX;
	int addY = dirY;

	bool interchange = false;

	addPrefab(x1, y1);
	int y = y1;
	int x = x1;
	int end = x2;
	if (dx < dy)
	{
		interchange = true;
		x = y1;
		end = y2;
		y = x1;
		deltaY = d2x;
		middle = dy;
		addMiddle = d2y;
		addDeltaY = d2x;
		addX = dirY;
		addY = dirX;
	}

	x += addX;
	while (x != end)
	{
		if (deltaY >= middle)
		{
			y += addY;
			middle += addMiddle;
		}

		if(interchange)
			addPrefab(y, x);
		else
			addPrefab(x, y);
		deltaY += addDeltaY;
		x += addX;
	}
	addPrefab(x2, y2);
}
```





