using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameOver : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

    }

    //ƒV[ƒ“‚ğ“Ç‚İ‚Ş
    public void Load()
    {
        SceneManager.LoadScene("GameStart");
    }
}
