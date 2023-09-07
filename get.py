import requests
import html
import sys

# This python script downloads data from https://api.fib.upc.edu

q = sys.argv[1]        #Something like '2023Q1'

url = 'https://api.fib.upc.edu/v2/quadrimestres/'+q+'/classes/?format=api&client_id=liSPe2KsaYUovErkk1WyqgMdYxOD1Wqd3VCXwhoy'

response = requests.get(url)
if response.status_code == 200:
    response_text = html.unescape(response.text)
    print(response_text)
    exit(0)
else:
    sys.stderr.write('The request to the FIB API failed with code ' + str(response.status_code) + '\n')
    exit(1)