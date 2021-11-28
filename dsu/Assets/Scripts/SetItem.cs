using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SetItem
{
    public int Id { get; }
    public GameObject Object { get; }

    public SetItem(int id, GameObject prefab)
    {
        Id = id;
        Object = new GameObject($"set_item_{id}");
    }
}
