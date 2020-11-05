import json

try:
    import requests
except ImportError:
    print("cannot import requests")


def download(uri):
    try:
        result = requests.get(uri)
        return json.dumps(result.json(), indent=4)
    except Exception:
        print('request failed')
        return None

