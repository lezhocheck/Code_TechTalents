using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public GameObject SetItemPrefab;
    public GameObject LinePrefab;
    
    public GameObject SidePanelContent;

    public Texture2D DefaultCursor;
    public Texture2D TakeCursor;

    public InputField field;

    public SetsController SetsController;
    
    [HideInInspector]
    public Camera MainCamera;

    [Header("Graph")]
    public Text NormalButtonText;
    public GameObject SidePanel, GraphPanel, GraphContent;
    public InputField verticesInput, edgesInput;
    private bool normalMode = true;
    private Graph graph;

    public Button unionButton, backButton, forwardButton;

    private void Start()
    {
        SetsController = new SetsController(this);
        Cursor.SetCursor(DefaultCursor, Vector2.zero, CursorMode.Auto);
        MainCamera = Camera.main;
        
        unionButton.onClick.AddListener(UnionClick);
        backButton.onClick.AddListener(PreviousClick);
        forwardButton.onClick.AddListener(NextClick);
    }

    public void UnionClick()
    {
        if (SetsController.GetSelected.Length == 0)
        {
            return;   
        }
        
        SetsController.ClearNextVersions();
        int[] selectedIndexes = 
            SetsController.GetSelected
            .Select(x=>x.Id).ToArray();

        bool allInSameSet = false;
        
        var all =  SetsController.GetAll(SetsController.Version);
        for (int i = 0; i + 1 < selectedIndexes.Length; i++)
        {
            int idF = SetsController.Dsu.Find(all[selectedIndexes[i]]).Id;
            int idS = SetsController.Dsu.Find(all[selectedIndexes[i + 1]]).Id;

            if (idF != idS)
            {
                allInSameSet = true;
            }
        }

        if (!allInSameSet)
        {
            return;
        }
        
        SetsController.AddNewVersion();
        
        all =  SetsController.GetAll(SetsController.Version);
        
        for (int i = 0; i + 1 < selectedIndexes.Length; i++)
        {
            Set a = all[selectedIndexes[i]];
            Set b = all[selectedIndexes[i + 1]];
            SetsController.Dsu.Union(a, b);
        }
        SetsController.DeselectAll(SetsController.Version - 1);
        SetsController.DeselectAll(SetsController.Version);
        SetsController.RedrawUi();
    }

    public void PreviousClick()
    {
        foreach (var item in SetsController.GetAll(SetsController.Version))
        {
            item.Hide();
        }

        SetsController.Version--;
        
        foreach (var item in SetsController.GetAll(SetsController.Version))
        {
            item.Show();
        }
        SetsController.RedrawUi();
    }
    
    public void NextClick()
    {
        foreach (var item in SetsController.GetAll(SetsController.Version))
        {
            item.Hide();
        }
        
        SetsController.Version++;
        
        foreach (var item in SetsController.GetAll(SetsController.Version))
        {
            item.Show();
        }
        SetsController.RedrawUi();
    }

    public void OnInputEntered()
    {
        if (field.text == "")
        {
            return;
        }
        int val = Int32.Parse(field.text);
        if (val > 100)
        {
            val = 100;
            field.text = val.ToString();
        }
        for (int i = 0; i < val; i++)
        {
            SetsController.CreateSet();
        }
        SetsController.RedrawUi();
        field.text = "";
        field.gameObject.SetActive(false);
    }

    public void ModeChangeButtonClick()
    {
        unionButton.onClick.RemoveAllListeners();
        forwardButton.onClick.RemoveAllListeners();
        normalMode = !normalMode;
        RectTransform rect = forwardButton.GetComponent<RectTransform>();
        if (normalMode)
        {
            unionButton.GetComponentInChildren<Text>().text = "Union";
            unionButton.onClick.AddListener(UnionClick);
            forwardButton.onClick.AddListener(NextClick);
            NormalButtonText.text = "Normal mode";
            forwardButton.GetComponentInChildren<Text>().text = "->";
            forwardButton.interactable = true;
            unionButton.interactable = true;
            rect.anchoredPosition = new Vector2(515f, -40f);
            rect.sizeDelta = new Vector2(180f, 50f);
        }
        else
        {
            unionButton.GetComponentInChildren<Text>().text = "Find MST";
            unionButton.onClick.AddListener(FindMst);
            forwardButton.onClick.AddListener(NextMstClick);
            NormalButtonText.text = "MST mode";
            forwardButton.GetComponentInChildren<Text>().text = "Next step";
            forwardButton.interactable = false;
            unionButton.interactable = false;
            rect.anchoredPosition = new Vector2(412.5f, -40f);
            rect.sizeDelta = new Vector2(385f, 50f);
        }
        SidePanel.SetActive(!SidePanel.activeSelf);
        GraphPanel.SetActive(!GraphPanel.activeSelf);
        backButton.gameObject.SetActive(!backButton.gameObject.activeSelf);
        verticesInput.text = "";
        edgesInput.text = "";
        graph?.Clear();
        SetsController.ClearAll();
    }

    private void FindMst()
    {
        graph.DisplayMst();
    }

    private void NextMstClick()
    {
        graph.NextStepMst();
    }
    
    public void CreateButtonClick()
    {
        graph?.Clear();
        
        if (!Int32.TryParse(verticesInput.text, out int vertices) ||
            edgesInput.text.Any(x=>Char.IsLetter(x)) ||
            vertices <= 1)
        {
            verticesInput.text = "";
            edgesInput.text = "";
            return;
        }
        List<Graph.Edge> edges = new List<Graph.Edge>();
        try
        {
            edges = ParseEdges(edgesInput.text, vertices);
        }
        catch (Exception)
        {
            verticesInput.text = "";
            edgesInput.text = "";
            return;
        }
        
        forwardButton.interactable = true;
        unionButton.interactable = true;
        graph = new Graph(this, vertices, 
            edges, new Vector2(0.0f, 0.0f), 
            3, GraphContent.GetComponent<RectTransform>());
        graph.Display();   
    }

    private List<Graph.Edge> ParseEdges(string text, int vertices)
    {
        text = Regex.Replace(text, @"[^0-9\s]", " ");
        text = Regex.Replace(text, @"\s+", " ");
        text = text.Trim();
        string[] numbers = text.Split(' ');
        if (numbers.Length % 3 != 0)
        {
            throw new ArgumentException("Invalid argument");
        }
        List<Graph.Edge> edges = new List<Graph.Edge>();
        for (int i = 0; i < numbers.Length; i += 3)
        {
            int start = Int32.Parse(numbers[i]);
            int finish = Int32.Parse(numbers[i + 1]);
            int weight = Int32.Parse(numbers[i + 2]);
            if (start >= vertices || finish >= vertices)
            {
                throw new ArgumentException("Invalid argument");
            }
            edges.Add(new Graph.Edge(start, finish, weight));
        }
        return edges;
    }
}
