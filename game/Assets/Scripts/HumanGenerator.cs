using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO;
using System.Linq;
using System.Threading;
using UnityEngine.SceneManagement;

public class HumanGenerator : MonoBehaviour
{
    public static GameObject obj;//private
    public List<GameObject> people;//人間取得配列
    public bool isFall;
    int file_length;
    public int pivotHeight = 7;//生成位置の基準

    void Init()
    {
        Human.isMoves.Clear();//移動してる人間のリストを初期化
        string[] files = Directory.GetFiles(
            @"Assets/Resources", "*.png", SearchOption.AllDirectories
            ).ToArray();
        file_length = files.Length;
        //       obj = null;
    }

    // Start is called before the first frame update
    void Start()
    {
        string[] files = Directory.GetFiles(
              @"Assets/Resources", "*.png", SearchOption.AllDirectories
              );
        foreach (string file in files)
        {
            File.SetAttributes(file, FileAttributes.Normal);
            File.Delete(file);
        }
        Init();
    }

    // Update is called once per frame
    void Update()
    {
        if (CheckGameOver(people))
        {
            SceneManager.LoadScene("GameOver");
        }
        if (CheckMove(Human.isMoves))
        {
            return;//移動中なら処理はここまで
        }
        if (ClearJudgment.clear)
        {
            ClearJudgment.clear = false;
            SceneManager.LoadScene("GameClear");
        }
        string[] files = Directory.GetFiles(
            @"Assets/Resources", "*.png", SearchOption.AllDirectories
            ).OrderBy(f => File.GetLastWriteTime(f).Date
            ).ToArray();
        if (files.Length == 0)
        {
            return;
        }
        if (files.Length > file_length)
        {
            string tar = files[files.Length - 1].Remove(0, 17);
            tar = tar.Replace(".png", "");
            Sprite img = Resources.Load(tar, typeof(Sprite)) as Sprite;
            //            Debug.Log(img);
            if (img == null)
            {
                return;
            }
            Create(img);
            file_length += 1;
        }


    }
    void Create(Sprite img)
    {
        isFall = false;
        obj = new GameObject();
        obj.AddComponent<SpriteRenderer>();
        obj.GetComponent<SpriteRenderer>().sprite = img;
        obj.AddComponent<PolygonCollider2D>();
        obj.AddComponent<Rigidbody2D>();
        obj.AddComponent<Human>();
        obj.transform.position = new Vector3(0.0f, pivotHeight, 0.0f);
        people.Add(obj);
    }

    /// <summary>
    /// 移動中かチェック
    /// </summary>
    /// <param name="isMoves"></param>
    /// <returns></returns>
    bool CheckMove(List<Moving> isMoves)
    {
        if (isMoves == null)
        {
            return false;
        }
        foreach (Moving b in isMoves)
        {
            if (b.isMove)
            {
                //Debug.Log("移動中");
                return true;
            }
        }
        return false;
    }

    bool CheckGameOver(List<GameObject> people)
    {
        foreach (GameObject b in people)
        {
            if (b.transform.position.y < -8)
            {
                return true;
            }
        }
        return false;
    }
}
