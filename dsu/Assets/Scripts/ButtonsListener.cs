using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonsListener : MonoBehaviour
{
    public GameObject SetItemPrefab;
    public GameObject SidePanelContent;
    public Font SetDisplayFont;

    private int lastSetId = 0;
    
    private List<Set> sets;

    private void Start()
    {
        sets = new List<Set>();
    }

    public void OnNewSetClick()
    {
        Set set = new Set(lastSetId, this);
        sets.Add(set);
        lastSetId++;
    }

    public void OnSetTextClick(Set set)
    {
        Debug.Log(set.Id);
    }
}
