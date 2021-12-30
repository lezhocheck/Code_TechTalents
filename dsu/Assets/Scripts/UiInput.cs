using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UiInput : MonoBehaviour, IPointerClickHandler
{
    private GameManager manager;
    
    void Start()
    {
        GameObject go = GameObject.Find("SceneManager");
        if (go == null)
        {
            throw new ApplicationException("No Scene Manager in scene");
        }

        manager = go.GetComponent<GameManager>();
    }
    

    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Right)
        {
            manager.field.gameObject.SetActive(false);
        }
    }
}
