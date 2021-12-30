using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;

[RequireComponent(typeof(SpriteRenderer))]
public class SetItem : MonoBehaviour
{
    [HideInInspector]
    public GameManager manager;
    public Set set;
    private SpriteRenderer spriteRenderer;
    private Camera cam;
    private float cameraZPosition;

    private Vector3 startPos;
    public void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
        cam = Camera.main;
        cameraZPosition = cam.ScreenToWorldPoint(transform.position).z;
    }

    private void OnMouseOver()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if (Input.GetButton("Left Control"))
            {
                Set[] selected = manager.SetsController.GetSelected;
                if (selected.Length > 1)
                {
                    Set first = selected[0];
                    for(int i = 1; i < selected.Length; i++)
                    {
                        selected[i].SetObject.transform.SetParent(first.SetObject.transform);
                    }
                }
                else
                {
                    manager.SetsController.DeselectAll( manager.SetsController.Version);
                }
            }
            else
            {
                manager.SetsController.DeselectAll(manager.SetsController.Version);
                spriteRenderer.color = Color.cyan;      
            }
        }

        if (Input.GetMouseButtonUp(0))
        {
            foreach (var s in manager.SetsController.GetSelected)
            {
                s.SetObject.transform.SetParent(null);   
            }
            spriteRenderer.color = Color.white;
        }
    }
    

    public void OnMouseDrag()
    {
        Vector3 screenPosition = new Vector3(Input.mousePosition.x, Input.mousePosition.y, -cameraZPosition);
        Vector3 newPosition = cam.ScreenToWorldPoint(screenPosition);
        Set[] selected = manager.SetsController.GetSelected;
        if (selected.Length > 1)
        {
            selected[0].SetObject.transform.position = newPosition; 
            return;
        }
        transform.position = newPosition; 
    }

    public void OnTriggerEnter2D(Collider2D other)
    {
        set.IsSelected = true;
    }
}
