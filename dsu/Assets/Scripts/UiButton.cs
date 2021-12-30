using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UiButton : MonoBehaviour, IPointerClickHandler
{
    private GameManager manager;
    private Camera cam;

    private void Start()
    {
        GameObject go = GameObject.Find("SceneManager");
        if (go == null)
        {
            throw new ApplicationException("No Scene Manager in scene");
        }

        manager = go.GetComponent<GameManager>();
        cam = Camera.main;
    }
    
    public void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button == PointerEventData.InputButton.Left)
        {
            manager.SetsController.CreateSet();
            manager.SetsController.RedrawUi();
        }
        else if (eventData.button == PointerEventData.InputButton.Right)
        {
            manager.field.gameObject.SetActive(true);
        }
    }
}
