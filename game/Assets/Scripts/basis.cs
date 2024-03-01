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

    //ƒV[ƒ“‚ğ“Ç‚İ‚Ş
    void Move()
    {
        // ¶‚ÉˆÚ“®
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            this.transform.Translate(-0.05f, 0.0f, 0.0f, Space.World);
        }
        // ‰E‚ÉˆÚ“®
        if (Input.GetKey(KeyCode.RightArrow))
        {
            this.transform.Translate(0.05f, 0.0f, 0.0f, Space.World);
        }
        
    }
}
