# 函数

```text
float circleSDF(vec2 st) {
    return length(st-.5)*2.;
}
```

```text
float stroke(float x, float size, float w) {
    float d = smoothstep(size-0.005,size+0.005, x+w*.5) - smoothstep(size-0.005,size+0.005, x-w*.5);
    return d;
}
```



