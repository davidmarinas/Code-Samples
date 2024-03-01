using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public int MaxMirrors;
    public static int mirrorsLeft;
    public GameObject mirrorPrefab;
    public static  GameObject selectedMirror;
    public GameObject ActivePanel;
    private List<GameObject> mirrors;
    public static List<GameObject> sunflowers = new List<GameObject>();
    public GameObject SpawnBox;
    public bool transitioning;

    public static bool fin;
    // Start is called before the first frame update
    void Awake()
    {
        fin = false;
        transitioning = false;
        selectedMirror = null;
        mirrors = new List<GameObject>();
        mirrorsLeft = MaxMirrors;
        ActivePanel.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (fin && !transitioning)
        {
            selectedMirror.GetComponent<Mirror>().movable = false;
            selectedMirror = null;
            DisableMirrors();
            transitioning = true;
            ActivePanel.SetActive(true);
            StartCoroutine(Next());
        }
    }

    private void LateUpdate()
    {
        if (sunflowers.Count == 0)
        {
            fin = true;
        }
    }

    public void SpawnMirror()
    {
        if (mirrors.Count < MaxMirrors && SpawnBox.transform.childCount == 0)
        {
            GameObject newMirror = Instantiate(mirrorPrefab,SpawnBox.transform);
            newMirror.transform.position = SpawnBox.transform.position;
            mirrors.Add(newMirror);
            mirrorsLeft--;
        }
    }

    public void DeleteMirror()
    {
        if (selectedMirror != null && selectedMirror.GetComponent<Mirror>().movable)
        {
            mirrors.Remove(selectedMirror);
            Destroy(selectedMirror);
            selectedMirror = null;
            mirrorsLeft++;
        }
    }
    public static void SelectMirror(GameObject obj)
    {
        if (!fin)
        {
            if (selectedMirror != null)
            {
                selectedMirror.GetComponent<SpriteRenderer>().color = Color.white;
            }
            selectedMirror = obj;
            selectedMirror.GetComponent<SpriteRenderer>().color = Color.yellow;
        }
    }

    public static void NextLevel()
    {
        fin = true;
    }
    
    static IEnumerator Next()
    {
        sunflowers.Clear();
        yield return new WaitForSeconds(2);
        if (SceneManager.GetActiveScene().buildIndex + 1 < SceneManager.sceneCountInBuildSettings)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }
    }

    public void DisableMirrors()
    {
        foreach (GameObject i in mirrors)
        {
            i.GetComponent<Mirror>().movable = false;
        }
    }
}
