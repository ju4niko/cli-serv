#!/usr/bin/env python3
from flask import Flask, request
import math

mi_aplicacion = Flask(__name__)

@mi_aplicacion.route('/medicion', methods=['POST'])
def posicion():
    if request.method == 'POST':
        data = request.get_json()  # Accede a los datos JSON
        val1 = data.get('val1')
        val2 = data.get('val2')

        print (f'Valor 1:{val1}')        
        print (f'Valor 2:{val2}')
    return "FIN\n"

if __name__ == '__main__':
    mi_aplicacion.run(host='0.0.0.0',port=17001, debug=True)

