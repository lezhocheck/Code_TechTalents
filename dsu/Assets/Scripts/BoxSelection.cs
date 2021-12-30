using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

[RequireComponent(typeof(SpriteRenderer))]
public class BoxSelection : MonoBehaviour
{
    [SerializeField]
    private GameManager manager;
    private SpriteRenderer spriteRenderer;
    private BoxCollider2D boxCollider;
    private Camera cam;

    private Vector2 startPos;

    private bool selectionStarted;

    void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
        boxCollider = GetComponent<BoxCollider2D>();
        spriteRenderer.enabled = false;
        boxCollider.enabled = spriteRenderer.enabled;
        cam = Camera.main;
    }
    
    void Update()
    {
        Vector2 mousePos = cam.ScreenToWorldPoint(Input.mousePosition);
        if (!selectionStarted && Physics2D.Raycast(mousePos, Vector2.zero, 10)) {
            Cursor.SetCursor(manager.TakeCursor, Vector2.zero, CursorMode.Auto);
        }
        else
        {
            Cursor.SetCursor(manager.DefaultCursor, Vector2.zero, CursorMode.Auto);
            
            if (!EventSystem.current.IsPointerOverGameObject())
            {
                if (Input.GetMouseButtonDown(0))
                {
                    StartSelection();
                }
            }
            if (Input.GetMouseButton(0))
            {
                UpdateSelection(mousePos);
            }
        }

        if (Input.GetMouseButtonUp(0))
        {
            ReleaseSelection();
        }
    }

    private void StartSelection()
    {
        manager.SetsController.DeselectAll(manager.SetsController.Version);
        if (!spriteRenderer.enabled)
        {
            spriteRenderer.enabled = true;
            boxCollider.enabled = spriteRenderer.enabled;
        }
        startPos = cam.ScreenToWorldPoint(Input.mousePosition);
        selectionStarted = true;
    }
    
    private void UpdateSelection(Vector2 currentMousePos)
    {
        float width = currentMousePos.x - startPos.x;
        float height = currentMousePos.y - startPos.y;

        spriteRenderer.size = new Vector2(Mathf.Abs(width), Mathf.Abs(height));
        spriteRenderer.transform.position = startPos + new Vector2(width / 2.0f, height / 2.0f);
        boxCollider.size = spriteRenderer.size;
    }

    private void ReleaseSelection()
    {
        foreach (var set in manager.SetsController.GetSelected)
        {
            set.Color = Color.cyan;
        }
        spriteRenderer.enabled = false;
        selectionStarted = false;
        boxCollider.enabled = spriteRenderer.enabled;
    }
}
