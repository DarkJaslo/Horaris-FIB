import requests
import html
import sys

q = sys.argv[1]        #Something like '2023Q1'

url = 'https://api.fib.upc.edu/v2/quadrimestres/'+q+'/classes/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy'

response = requests.get(url)
if response.status_code == 200:
    response_text = html.unescape(response.text)
    print(response_text)
else:
    print('Request failed:', response.status_code)