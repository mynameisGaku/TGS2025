using UnityEngine;

public class BallSpawnerDesc : MonoBehaviour
{
    [SerializeField]
    public float INTERVAL_SEC = 0.0f;
    [SerializeField]
    public float INTERVAL_SEC_RANDOM_RANGE = 0.0f;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_MAX = 0;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_MIN = 0;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_RANDOM_RANGE = 0;
    [SerializeField]
    public int SPAWN_AMOUNT_INITIAL = 0;
    [SerializeField]
    public Vector3 SPAWN_RANGE = Vector3.zero;
    [SerializeField]
    public Vector3 SPAWN_FIRST_VELOCITY = Vector3.zero;
    [SerializeField]
    public Vector3 SPAWNER_POSITION = Vector3.zero;
    [SerializeField]
    public string SPAWNER_NAME = "NO NAME SPAWNER.";
}
