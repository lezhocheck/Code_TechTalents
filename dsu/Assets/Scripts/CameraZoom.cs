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

    private Camera cam;
    private Vector3 dragOrigin;
    
    private enum MouseButtons
    {
        Left,
        Right,
        Wheel
    }
    
    void Start()
    {
        cam = gameObject.GetComponent<Camera>();
    }
    
    void Update()
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
            cam.transform.position += difference;
        }
    }
}
