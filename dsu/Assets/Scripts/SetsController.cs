using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;
using Object = UnityEngine.Object;
using Random = UnityEngine.Random;

public class SetsController
{
    private readonly GameManager manager;

    private readonly List<Dsu> versions;

    private int version;
    public int Version 
    {
        get => version;
        set
        {
            if (value < 0) version = 0;
            else if (value >= versions.Count) version = versions.Count - 1;
            else version = value;
        }
    }
    
    public Dsu Dsu => versions[Version];
    public Set[] GetAll(int value) => versions[value].Items.Select(x => x.set).ToArray();
    public Set[] GetSelected => Dsu.Items
        .Where(x => x.set.IsSelected)
        .Select(x=>x.set).ToArray();
    
    public SetsController(GameManager manager)
    {
        versions = new List<Dsu>();
        versions.Add(new Dsu(manager));
        this.manager = manager;
    }

    public void AddNewVersion()
    {
        versions.Add(Dsu.Clone());
        Version++;
    }

    public void ClearAll()
    {
        foreach (var dsu in versions)
        {
            dsu.Clear();
        }
        versions.Clear();
        versions.Add(new Dsu(manager));
        Version = 0;
        RedrawUi();
    }

    public void ClearNextVersions()
    {
        for (int i = Version + 1; i < versions.Count; i++)
        {
            versions[i].Clear();
        }
        versions.RemoveRange(Version + 1, versions.Count - Version - 1);
    }
    
    private void Display(Vector3 position)
    {
        int index = Dsu.Items.Count;
        
        float rX = Random.Range(0.0f, 0.1f);
        float rY = Random.Range(0.0f, 0.1f);
        position = new Vector3(position.x + rX, position.y + rY);
        Set set = new Set(index, position, manager);
        Dsu.AddNew(set);
    }

    private List<Set> GetPresentersInBegin()
    {
        List<Set> list = new List<Set>();
        Set[] all = GetAll(Version);
        foreach (var set in all)
        {
            if (Dsu.IsPresenter(set))
            {
                list.Add(set);
            }
        }
        
        foreach (var set in all)
        {
            if (!Dsu.IsPresenter(set))
            {
                list.Add(set);
            }
        }

        return list;
    }

    public void CreateSet()
    {
        Vector3 camPos = manager.MainCamera.transform.position;
        camPos.Set(camPos.x, camPos.y, 0.0f);
        Display(camPos);
    }
    
    public void AddLine(Set parent, Set child)
    {
        GameObject obj = Object.Instantiate(manager.LinePrefab,
            child.SetObject.transform.position, 
            Quaternion.identity, child.SetObject.transform);
        Line line = obj.GetComponent<Line>();
        line.FinishObject = parent;
    }
    
    public void DeselectAll(int versionValue)
    {
        Set[] selectedArray = GetAll(versionValue);
        if (selectedArray.Length == 0)
        {
            return;
        }

        foreach (var set in selectedArray)
        {
            set.Color = Color.white;
            set.IsSelected = false;
        }
    }

    public void RedrawUi()
    {
        RectTransform panel = manager.SidePanelContent.GetComponent<RectTransform>();
        foreach (Transform child in panel)
        {
            Object.Destroy(child.gameObject);
        }

        panel.sizeDelta = new Vector2(panel.sizeDelta.x, 0.0f);
        Dropbox.LastAdded = 0;
        List<Set> list = GetPresentersInBegin();
        List<Dropbox> dropboxes = new List<Dropbox>();
        foreach (var set in list)
        {
            if (Dsu.IsPresenter(set))
            {
                dropboxes.Add(new Dropbox(set, panel));
            } 
            else
            {
                Dsu.DsuItem item = Dsu.GetItemBySet(set);
                var box = dropboxes.Find(
                    x => x.MainSet.Id == 
                          Dsu.Find(item.parent).Id);
                box.AddSet(item.set);
            }
        }
    }
}
