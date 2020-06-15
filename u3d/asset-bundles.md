# Asset Bundles

{% embed url="https://learn.unity.com/tutorial/introduction-to-asset-bundles\#5ce589b4edbc2a106aa7b47c" %}

{% embed url="https://www.cnblogs.com/88999660/archive/2013/03/15/2961663.html" %}

```text
using System.Collections;
using System.IO;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;

/// <summary> 加载测试 </summary>
public class LoadTest : MonoBehaviour {
    
    void OnGUI()
    {
        if(GUILayout.Button("LoadFromFile Assetbundle"))
        {
            LoadFromFile();
        }
 
        if(GUILayout.Button("LoadFromFileAsync Assetbundle"))
        {
            StartCoroutine(LoadFromFileAsync());
        }
        
        if(GUILayout.Button("LoadFromWWW Assetbundle"))
        {
            StartCoroutine(LoadFromWWW());
        }

        if(GUILayout.Button("LoadFromUnityWebRequest Assetbundle"))
        {
            StartCoroutine(LoadFromUnityWebRequest());
        }
    }

    void Start()
    {
        

    }

    public string assetName = "BundledSpriteObject";
    public string bundleName = "testbundle";

    public void LoadFromFile()
    {
        AssetBundle localAssetBundle = AssetBundle.LoadFromFile(Path.Combine(Application.streamingAssetsPath, bundleName));

        if (localAssetBundle == null)
        {
            Debug.LogError("Failed to load AssetBundle!");
            return;
        }

        GameObject asset = localAssetBundle.LoadAsset<GameObject>(assetName);
        Instantiate(asset);
        localAssetBundle.Unload(false);
    }

    IEnumerator LoadFromFileAsync()
    {
        AssetBundleCreateRequest asyncBundleRequest =
            AssetBundle.LoadFromFileAsync(Path.Combine(Application.streamingAssetsPath, bundleName));
        yield return asyncBundleRequest;

        AssetBundle localAssetBundle = asyncBundleRequest.assetBundle;
        
        if (localAssetBundle == null)
        {
            Debug.LogError("Failed to load AssetBundle!");
            yield break;
        }

        AssetBundleRequest assetRequest = localAssetBundle.LoadAssetAsync<GameObject>(assetName);
        yield return assetRequest;

        GameObject prefab = assetRequest.asset as GameObject;

        Instantiate(prefab);
        localAssetBundle.Unload(false);
    }

    
    IEnumerator LoadFromWWW()
    {
        using (WWW web = new WWW(Path.Combine(Application.streamingAssetsPath, bundleName)))
        {
            yield return web;
            AssetBundle remoteAssetBundle = web.assetBundle;
            if (remoteAssetBundle == null)
            {
                Debug.LogError("Failed to download AssetBundle!");
            }

            Instantiate(remoteAssetBundle.LoadAsset(assetName));
            remoteAssetBundle.Unload(false);

        }
    }
    
    IEnumerator LoadFromUnityWebRequest()
    {
        string uri = "file:///" + Path.Combine(Application.streamingAssetsPath, bundleName);        
        UnityWebRequest request = UnityWebRequestAssetBundle.GetAssetBundle(uri);
        yield return request.SendWebRequest();
        AssetBundle bundle = DownloadHandlerAssetBundle.GetContent(request);
        GameObject cube = bundle.LoadAsset<GameObject>(assetName);
        Instantiate(cube);
    }


}
```



