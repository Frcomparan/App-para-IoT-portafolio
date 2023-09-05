import requests
import os
from dotenv import load_dotenv
load_dotenv()

API_KEY = os.getenv("API_KEY")

url = "https://google-translate1.p.rapidapi.com/language/translate/v2"

payload = {
	"q": "Hello, furry world!",
	"target": "es",
	"source": "en"
}
headers = {
	"content-type": "application/x-www-form-urlencoded",
	"Accept-Encoding": "application/gzip",
	"X-RapidAPI-Key": API_KEY,
	"X-RapidAPI-Host": "google-translate1.p.rapidapi.com"
}

response = requests.post(url, data=payload, headers=headers)

print(response.json())