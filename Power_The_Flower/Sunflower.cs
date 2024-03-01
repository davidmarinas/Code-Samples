using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sunflower : MonoBehaviour
{
    private Animator anim;

    private bool transitioned = false;

    public bool colliding = false;
    
    // Start is called before the first frame update
    void Awake()
    {
        GameManager.sunflowers.Add(gameObject);
        anim = gameObject.GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        if (colliding)
        {
            SunIn();
        }
        else
        {
            SunOut();
        }

        colliding = false;
    }
    private void SunIn()
    {
        if (!transitioned)
        {
            anim.SetBool("Bloom",true);
            transitioned = true;
        }
        else
        {
            anim.speed = 1;
        }
    }
    private void SunOut()
    {
        anim.speed = 0;
    }

    private void Ended()
    {
        GameManager.sunflowers.Remove(gameObject);
    }
}
