using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(LineRenderer))]
public class Line : MonoBehaviour
{
    public Set FinishObject { get; set; }
    
    private LineRenderer line;
    void Start()
    {
        line = GetComponent<LineRenderer>();
    }
    
    void Update()
    {
        Vector3[] pos = { gameObject.transform.position, FinishObject.SetObject.gameObject.transform.position};
        line.SetPositions(pos);
    }
}
