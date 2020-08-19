# 函数

```text
float aastep(float threshold, float value) {
	return smoothstep(threshold-0.005,threshold+0.005, value);
}
```

```text
float circleSDF(vec2 st) {
    return length(st-.5)*2.;
}
```

```text
float stroke(float x, float size, float w) {
    float d = aastep(size,x+w*.5) - aastep(size, x-w*.5);
    return d;
}
```



