using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class basis : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        Move();
    }

    //シーンを読み込む
    void Move()
    {
        // 左に移動
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            this.transform.Translate(-0.05f, 0.0f, 0.0f, Space.World);
        }
        // 右に移動
        if (Input.GetKey(KeyCode.RightArrow))
        {
            this.transform.Translate(0.05f, 0.0f, 0.0f, Space.World);
        }
        
    }
}
