using UnityEngine;

public class BallSpawnerDesc : MonoBehaviour
{
    [SerializeField]
    public float INTERVAL_SEC = 1.0f;
    [SerializeField]
    public float INTERVAL_SEC_RANDOM_RANGE = 0.5f;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_MAX = 1;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_MIN = 1;
    [SerializeField]
    public int SPAWN_AMOUNT_ONCE_RANDOM_RANGE = 0;
    [SerializeField]
    public int SPAWN_AMOUNT_INITIAL = 0;
    [SerializeField]
    public Vector3 SPAWN_RANGE = Vector3.zero;
    [SerializeField]
    public Vector3 SPAWN_INITIAL_VELOCITY = Vector3.zero;
}
