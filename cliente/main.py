from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles

app = FastAPI()
app.mount("/static", StaticFiles(directory="static"), name="static")
templates = Jinja2Templates(directory="templates")

# Configuración de CORS
origins = [
    "http://localhost",
    "http://127.0.0.1:8000",  # Agrega los dominios permitidos aquí
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],  # Permite todos los métodos HTTP (GET, POST, etc.)
    allow_headers=["*"],  # Permite todas las cabeceras HTTP
)

@app.get("/")
def index(request: Request):
    return templates.TemplateResponse("pagina.html", context={"request": request})

@app.get("/alive")
def renovarVida():
    return True

