#Instalar pacotes: sudo pip3 install pandas; sudo pip3 install jinja2; sudo pip3 install openpyxl

import pandas as pd
import numpy as np
from pandas.io.formats.style import Styler

dados_col = ['Imagem', 'Area', 'AnguloR', 'Angulo', 'Distancia', '% de sucesso']
df = pd.read_table('Resultados/1.txt', sep = ' ', header = None, names=dados_col)
df['Imagem'] = df['Imagem'].astype(str)

df_styled=df.style\
    .applymap(lambda x: 'background-color: %s' % 'red' if x > 5000 else 'background: %s' % '', subset=['Area'])\
    .applymap(lambda x: 'background-color: %s' % 'red' if x > 30 else 'background: %s' % ' ', subset=['AnguloR'])\
    .applymap(lambda x: 'background-color: %s' % 'red' if x < 55 else 'background: %s' % ' ', subset=['Angulo'])\
    .applymap(lambda x: 'background-color: %s' % 'red' if x > 130 else 'background: %s' % ' ', subset=['Angulo'])\
    .applymap(lambda x: 'background-color: %s' % 'None' if x == 0.0 else 'background: %s' % 'Yellow', subset=['Angulo'])\
    .applymap(lambda x: 'background-color: %s' % 'red' if x > 50  else 'background: %s' % ' ', subset=['Distancia'])\
    .format({'AnguloR': "{:.2f}"})\
    .format({'Angulo': "{:.2f}"})\
    .format({'% de sucesso': "{:.2f}"})\
    .set_properties(**{'text-align': 'right'})\

df_styled.to_excel('Resultados.xlsx', engine='openpyxl', index=False)