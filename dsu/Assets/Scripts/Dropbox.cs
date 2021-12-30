using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Object = UnityEngine.Object;

public class Dropbox
{
    public Set MainSet { get; }
    private readonly Dropdown dropdown;
    public static int LastAdded { get; set; }

    public Dropbox(Set set, RectTransform parent)
    {
        MainSet = set;
        GameObject prefab = Resources.Load("Prefabs/Dropdown") as GameObject;
        if (prefab == null)
        {
            throw new ApplicationException("No dropdown prefab");
        }

        Vector2 parameters = prefab.GetComponent<RectTransform>().sizeDelta;
        
        GameObject go = Object.Instantiate(prefab, 
            Vector3.zero, Quaternion.identity, parent);
        dropdown = go.GetComponent<Dropdown>();
        RectTransform transform = go.GetComponent<RectTransform>();

        transform.anchoredPosition = new Vector2(0.0f, -parameters.y * LastAdded);
        LastAdded++;
        parent.sizeDelta += new Vector2(0.0f, parameters.y);

        dropdown.options.Clear();
        dropdown.options.Add(new Dropdown.OptionData($"Set {MainSet.Id}"));
    }

    public void AddSet(Set set)
    {
        dropdown.options.Add(new Dropdown.OptionData($"Set {set.Id}"));  
    }
}
