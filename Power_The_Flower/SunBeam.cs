using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sunbeam : MonoBehaviour
{
    public LineRenderer LineOfSight;
    private List<Vector2> beamIndices = new List<Vector2>();
    public LayerMask LayerDetection;
    private RaycastHit2D hitInfo;
    private List<GameObject> mirrorsHit = new List<GameObject>();
    // Start is called before the first frame update
    void Awake()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        LineOfSight.positionCount = 0;
        beamIndices.Clear();
        mirrorsHit.Clear();
        CastRay(transform.position,transform.right);
    }
    void CastRay(Vector2 pos, Vector2 dir)
    {
        beamIndices.Add(pos);
        Ray2D ray = new Ray2D(pos, dir);
        hitInfo = Physics2D.Raycast(pos, dir, 30, LayerDetection);
        if (hitInfo.collider != null)
        {
            if (hitInfo.collider.CompareTag("Mirror") && !mirrorsHit.Contains(hitInfo.transform.gameObject))
            {
                dir = Vector2.Reflect(dir, hitInfo.normal);
                pos = hitInfo.point + dir * 0.01f;
                mirrorsHit.Add(hitInfo.transform.gameObject);
                CastRay(pos,dir);
            }
            else if(hitInfo.collider.CompareTag("Sunflower"))
            {
                hitInfo.transform.gameObject.GetComponent<Sunflower>().colliding = true;
                beamIndices.Add(ray.GetPoint(30));
                UpdateLaser();
            }
            else
            {
                beamIndices.Add(hitInfo.point);
                UpdateLaser();
            }
        }
        else
        {
            {
                beamIndices.Add(ray.GetPoint(30));
                UpdateLaser();
            }
        }
    }

    void UpdateLaser()
    {
        int count = 0;
        LineOfSight.positionCount = beamIndices.Count;

        foreach (Vector3 idx in beamIndices)
        {
            LineOfSight.SetPosition(count,idx);
            count++;
        }
    }
    
}

