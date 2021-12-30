using System;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;
using Object = UnityEngine.Object;
using Random = UnityEngine.Random;

public class Graph
{
    public class Edge : IComparable
    {
        public int Weight { get; }
        public int StartVertex { get; }
        public int EndVertex { get; }

        public Edge(int startVertex, int endVertex, int weight)
        {
            StartVertex = startVertex;
            EndVertex = endVertex;
            Weight = weight;
        }

        public override bool Equals(object obj)
        {
            if (!(obj is Edge)) return false;

            Edge edge = (Edge)obj;
            return (StartVertex == edge.StartVertex 
                   && EndVertex == edge.EndVertex) ||
                   (StartVertex == edge.EndVertex 
                    && EndVertex == edge.StartVertex);
        }

        public override int GetHashCode()
        {
            return StartVertex.GetHashCode() ^ 
                   EndVertex.GetHashCode();
        }

        public int CompareTo(object obj)
        {
            Edge e = obj as Edge;
            if (e != null)
                return Weight.CompareTo(e.Weight);
            
            throw new Exception("Error");
        }
    }

    private readonly Vector2 startPoint;
    private readonly float radius;
    private readonly int verticesCount;
    private readonly List<RectTransform> verticesTransform; 
    private readonly List<Edge> edges;
    private readonly GameObject graphItemPrefab;
    private readonly RectTransform parent;
    private readonly Sprite edgeSprite;
    private readonly Font textFont;

    private Dictionary<Edge, Image> edgeLines;
    private GameManager manager;
    private Dsu dsu;
    
    public Graph(GameManager manager, int verticesCount, List<Edge> edges,
        Vector2 startPoint, float radius, RectTransform parent)
    {
        this.verticesCount = verticesCount;
        verticesTransform = new List<RectTransform>();
        this.edges = edges;
        this.startPoint = startPoint;
        graphItemPrefab = Resources.Load("Prefabs/graphItem") as GameObject;
        this.radius = CalculateRadius(radius, graphItemPrefab.GetComponent<RectTransform>().sizeDelta.x, verticesCount);
        GameObject go = new GameObject("graph", typeof(RectTransform));
        go.transform.SetParent(parent, false);
        RectTransform p = go.GetComponent<RectTransform>();
        p.anchoredPosition = startPoint;
        this.parent = p;

        Texture2D text = Resources.Load("Graphics/Square") as Texture2D;
        if (text == null)
        {
            throw new ApplicationException("No square texture");
        }
        edgeSprite = Sprite.Create(text, new Rect(0f, 0f, text.width, text.height), Vector2.zero);
        textFont = Resources.GetBuiltinResource(typeof(Font), "Arial.ttf") as Font;
        this.manager = manager;
        dsu = new Dsu(manager);
        edgeLines = new Dictionary<Edge, Image>();
    }

    public void Display()
    {
        DisplayVertices();
        DisplayEdges();
    }

    public void Clear()
    {
        verticesTransform.Clear();
        edgeLines.Clear();
        dsu.Clear();
        edges.Clear();
        if (parent)
        {
            Object.Destroy(parent.gameObject);   
        }
    }

    private float CalculateRadius(float radiusValue, float figureSize, int vertices)
    {
        float sizeOfCircle = 2 * Mathf.PI * radiusValue;
        float val = sizeOfCircle / vertices * figureSize;
        if (val < figureSize + 3f)
        {
            radiusValue *= (figureSize / val);
        }
        return radiusValue * figureSize;
    }
    
    private void DisplayVertices()
    {
        if (verticesCount > 100)
        {
            throw new ApplicationException("Too many vertices");
        }
        
        float segmentDegrees = 360f / verticesCount;
        Vector2 initial = startPoint + (Vector2.up * radius);
        for (int i = 0; i < verticesCount; i++)
        {
            float angle = i * segmentDegrees;
            Vector3 point = Quaternion.Euler(0f, 0f, angle) * initial;
            GameObject go = Object.Instantiate(graphItemPrefab, Vector3.zero, Quaternion.identity, parent);
            RectTransform rectTransform = go.GetComponent<RectTransform>();
            verticesTransform.Add(rectTransform);
            rectTransform.anchoredPosition = point;
            go.GetComponentInChildren<Text>().text = i.ToString();
        }

        if (verticesCount < 20)
        {
            parent.localScale *= 1.4f;
        }
        else if (verticesCount > 70)
        {
            parent.localScale *= 0.45f;
        }
        else if (verticesCount > 45)
        {
            parent.localScale *= 0.7f;
        }
    }

    private void DisplayEdges()
    {
        GameObject obj = new GameObject("lines", typeof(RectTransform));
        RectTransform transform = obj.GetComponent<RectTransform>();
        transform.SetParent(parent);
        transform.localScale = Vector3.one;
        transform.SetSiblingIndex(0);
        transform.anchoredPosition = Vector2.zero;
        foreach (var edge in edges.Distinct())
        {
            CreateLine(obj.transform, edge, 3f, Color.cyan);
        }
    }
    
    private void CreateLine(Transform lines, 
        Edge edge, float width, Color color) {
        GameObject obj = new GameObject($"line_{edge.StartVertex}_{edge.EndVertex}", 
            typeof(Image));
        Image image = obj.GetComponent<Image>();
        image.sprite = edgeSprite;
        image.color = color;
        RectTransform rect = obj.GetComponent<RectTransform>();
        rect.SetParent(lines);
        rect.localScale = Vector3.one;
        edgeLines.Add(edge, image);

        Vector2 a = verticesTransform[edge.StartVertex].anchoredPosition;
        Vector2 b = verticesTransform[edge.EndVertex].anchoredPosition;
        rect.localPosition = (a + b) / 2;
        
        GameObject goText = new GameObject("weight", typeof(Text));
        goText.transform.SetParent(rect);
        goText.transform.localScale = Vector3.one;
        Text text = goText.GetComponent<Text>();
        text.color = Color.white;
        text.alignment = TextAnchor.MiddleCenter;
        text.font = textFont;
        text.fontSize = 16;
        text.text = edge.Weight.ToString();
        RectTransform textTransform = goText.GetComponent<RectTransform>();
        textTransform.anchoredPosition = rect.localPosition;
        textTransform.anchoredPosition = new Vector2(-10f, -10f);
        Vector3 dif = a - b;
        rect.sizeDelta = new Vector3(dif.magnitude, width);
        rect.rotation = Quaternion.Euler(new Vector3(0, 0, 
            180 * Mathf.Atan(dif.y / dif.x) / Mathf.PI));
    }

    public void DisplayMst(Vector3 cameraPosition)
    {
        List<Edge> list = Mst(cameraPosition);
        foreach (var edge in list)
        {
            edgeLines[edge].color = Color.red;
        }
    }
    
    private List<Edge> Mst(Vector3 position)
    {
        for (int i = 0; i < verticesCount; i++)
        {
            float rX = Random.Range(0.0f, 2f);
            float rY = Random.Range(0.0f, 2f);
            Vector3 pos = new Vector3(position.x + rX, position.y + rY);
            Set set = new Set(i, pos, manager);
            dsu.AddNew(set);
        }

        List<Edge> sorted = new List<Edge>();
        foreach (var edge in edges)
        {
            sorted.Add(new Edge(edge.StartVertex, edge.EndVertex, edge.Weight));
        }
        sorted.Sort();
        List<Edge> result = new List<Edge>();
        foreach (var edge in sorted)
        {
            Set start = dsu.Items[edge.StartVertex].set;
            Set finish = dsu.Items[edge.EndVertex].set;
            if (dsu.Find(start).Id != dsu.Find(finish).Id)
            {
                result.Add(edge);
                dsu.Union(start, finish);   
            }
        }

        return result;
    }
}
