using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class Mirror : MonoBehaviour
{
    private float deltaX, deltaY;

    private Vector2 oldPos;
    private Vector2 mousePos;
    private bool colliding = false;
    private bool inSpawn = true;
    private Transform parent;
    public bool movable;
    
    // Start is called before the first frame update
    void Start()
    {
        oldPos = transform.position;
        parent = transform.parent;
    }
    

    public void OnMouseDown()
    {
        GameManager.SelectMirror(gameObject);
        if (movable)
        {
            oldPos = transform.position;
            deltaX = Camera.main.ScreenToWorldPoint(Input.mousePosition).x - transform.position.x;
            deltaY = Camera.main.ScreenToWorldPoint(Input.mousePosition).y - transform.position.y;
        }
    }

    public void OnMouseDrag()
    {
        if (movable)
        {
            if (transform.parent != null)
            {
                transform.parent = null;
            }
            mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            transform.position = new Vector2(mousePos.x - deltaX, mousePos.y - deltaY);
        }
    }

    public void OnMouseUp()
    {
        if (movable)
        {
            if (colliding || !gameObject.GetComponent<Renderer>().isVisible)
            {
                transform.position = oldPos;
                if (inSpawn)
                {
                    transform.parent = parent;
                }
            }
            else
            {
                if (inSpawn)
                {
                    inSpawn = false;
                }
                oldPos = transform.position;
            }
        }
        
    }
    
    private void OnTriggerEnter2D(Collider2D col)
    {
        colliding = true;
    }
    
    private void OnTriggerExit2D(Collider2D col)
    {
        colliding = false;
    }
}
