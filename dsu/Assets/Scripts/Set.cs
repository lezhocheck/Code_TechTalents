using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Set
{
    private const string DISPLAY_PANEL = "SetsDisplayPanel";
    public int Id { get; }
    public List<SetItem> Items { get; }
    public Text SetText { get; }
    public GameObject SetObject { get; }
    
    private readonly ButtonsListener listener;

    public Set(int id, ButtonsListener listener)
    {
        Id = id;
        Items = new List<SetItem>();
        this.listener = listener;
        SetObject = new GameObject($"set_{id}");
        SetText = AddSetText(listener.SidePanelContent, listener.SetDisplayFont);
    }

    private Text AddSetText(GameObject parent, Font font)
    {
        GameObject textObj = new GameObject(SetObject.name, 
            typeof(RectTransform), typeof(Text), typeof(Button));

        RectTransform transform = textObj.GetComponent<RectTransform>();
        Text text = textObj.GetComponent<Text>();
        Button button = textObj.GetComponent<Button>();

        RectTransform parentTransform = parent.GetComponent<RectTransform>();
        parentTransform.sizeDelta = parentTransform.sizeDelta + new Vector2(0.0f, 40.0f);  
        textObj.transform.SetParent(parent.transform, false);
        
        Vector2 parentSize = transform.parent.GetComponent<RectTransform>().rect.size;
        transform.sizeDelta = new Vector2(parentSize.x - 40.0f, 40.0f);
        transform.anchorMin = new Vector2(0.5f, 1.0f);
        transform.anchorMax = transform.anchorMin;
        transform.pivot = new Vector2(0.5f, 1.0f);
        transform.localPosition -= new Vector3(0.0f, transform.rect.height * Id, 0.0f);  
        
        text.alignment = TextAnchor.MiddleLeft;
        text.text = $"Set number {Id}";
        text.font = font;
        text.fontSize = 30;
        text.color = Color.white;
        
        button.onClick.AddListener(() => listener.OnSetTextClick(this));
        return text;
    }
}
