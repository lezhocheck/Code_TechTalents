using System;
using UnityEngine;


[RequireComponent(typeof(Camera))]
public class CameraZoom : MonoBehaviour
{
    [Header("Zoom settings")]
    [Range(0.0f, 10.0f)]
    [SerializeField]
    private float zoomSensitivity;
    [SerializeField]
    private float minZoom;
    [SerializeField]
    private float maxZoom;

    [Header("Movement settings")] 
    [SerializeField]
    private MouseButtons mouseButton;

    [Header("Movement settings")] 
    [SerializeField]
    private MeshRenderer grid;
    
    private Camera cam;
    private Vector3 dragOrigin;

    private Vector2 gridMin;
    private Vector2 gridMax;

    private enum MouseButtons
    {
        Left,
        Right,
        Wheel
    }
    
    private void Start()
    {
        cam = gameObject.GetComponent<Camera>();

        gridMin.x = grid.transform.position.x - grid.bounds.size.x / 2.0f;
        gridMin.y = grid.transform.position.y - grid.bounds.size.y / 2.0f;        
        gridMax.x =  grid.transform.position.x + grid.bounds.size.x / 2.0f;
        gridMax.y =  grid.transform.position.y + grid.bounds.size.y / 2.0f;
    }
    
    private void Update()
    {
        float size = cam.orthographicSize;
        size -= Input.GetAxis("Mouse ScrollWheel") * zoomSensitivity;
        size = Mathf.Clamp(size, minZoom, maxZoom);
        cam.orthographicSize = size;

        if (Input.GetMouseButtonDown((int)mouseButton))
        {
            dragOrigin = cam.ScreenToWorldPoint(Input.mousePosition);
        }
        
        if (Input.GetMouseButton((int)mouseButton))
        {
            Vector3 difference = dragOrigin - cam.ScreenToWorldPoint(Input.mousePosition);
            cam.transform.position = ClampCamera(cam.transform.position + difference);   
        }
    }

    private Vector3 ClampCamera(Vector3 targetPosition)
    {
        Vector2 camParameters = new Vector2(cam.orthographicSize * cam.aspect, cam.orthographicSize);
        Vector2 min = gridMin + camParameters;
        Vector2 max = gridMax - camParameters;

        float newX = Mathf.Clamp(targetPosition.x, min.x, max.x);
        float newY = Mathf.Clamp(targetPosition.y, min.y, max.y);
        return new Vector3(newX, newY, targetPosition.z);
    }
}
