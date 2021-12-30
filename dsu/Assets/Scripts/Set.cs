using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;
using Object = UnityEngine.Object;

public class Set
{
    public int Id { get; private set; }
    public GameObject SetObject { get; }
    
    public bool IsSelected { get; set; }

    private GameManager manager;

    public Color Color
    {
        set => SetObject.GetComponent<SpriteRenderer>().color = value;
    }

    public Set(int id, Vector3 position, GameManager manager)
    {
        Id = id;
        this.manager = manager;
        SetObject = AddItem(manager.SetItemPrefab, position);
    }
    
    private GameObject AddItem(GameObject prefab, Vector3 position)
    {
        GameObject obj = Object.Instantiate(prefab);
        obj.transform.position = position;
        obj.name = $"set_{Id}";
        SetItem item = obj.GetComponent<SetItem>();
        item.manager = manager;
        item.set = this;
        TextMeshPro text = obj.GetComponentInChildren<TextMeshPro>();
        text.text = Id.ToString();
        return obj;
    }

    public Set Clone()
    {
        Set set = new Set(Id, SetObject.transform.position, manager);
        return set;
    }

    public void Hide()
    {
        SetObject.SetActive(false);
    }

    public void Show()
    {
        SetObject.SetActive(true);
    }

    private Dropbox AddSetText(GameObject parent, Font font)
    {
        RectTransform paRectTransform = parent.GetComponent<RectTransform>();
        Dropbox dropBox = new Dropbox(this, paRectTransform);
        return dropBox;
    }

    public void Delete()
    {
        Object.Destroy(SetObject);
        Id = -1;
        IsSelected = false;
        manager = null;
    }
}
