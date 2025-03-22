import argparse
import json
import random

parser = argparse.ArgumentParser(
    description="Generatore di dati per corso di lab di algoritmi"
)
parser.add_argument("size", type=int, help="Size del dataset da generare")
parser.add_argument(
    "range_start", type=int, help="Inizio intervallo a cui appartengono i dati"
)
parser.add_argument(
    "range_end", type=int, help="Fine intervallo a cui appartengono i dati"
)
parser.add_argument(
    "--tries",
    default=1,
    type=int,
    help="Numero di dataset da generare, utile per dataset random.",
)
args = parser.parse_args()

size = args.size
start = args.range_start
end = args.range_end + 1  # inclusive
encoded = {
    "sorted": [i for i in range(size)],
    "sorted_rev": [i for i in range(size, 0, -1)],
    "random": {
        args.tries: [
            [random.randrange(start, end) for _ in range(size)]
            for _ in range(args.tries)
        ]
    },
}

print(json.dumps(encoded))
