def golomb_encode(number, M):
    # Paso 1: Calcular el cociente y el resto
    quotient = number // M
    remainder = number % M

    # Paso 2: Codificación unaria del cociente
    unary_code = "1" * quotient + "0"

    # Paso 3: Codificación binaria del resto
    binary_code = bin(remainder)[2:]  # Convertir a binario y eliminar el prefijo '0b'

    # Ajustar la longitud de la cadena binaria si es necesario
    while len(binary_code) < M.bit_length() - 1:
        binary_code = '0' + binary_code

    # Paso 4: Combinar la codificación unaria y binaria para obtener el código Golomb
    golomb_code = unary_code + binary_code

    # Paso 5: Devolver el código Golomb resultante
    return golomb_code

# Ejemplo de uso
print(golomb_encode(10, 4))

def golomb_decode(golomb_code, M):
    unary_length = golomb_code.find("0") + 1
    quotient = unary_length - 1

    binary_code = golomb_code[unary_length:unary_length + M.bit_length() - 1]
    remainder = int(binary_code, 2)

    decoded_number = quotient * M + remainder
    return decoded_number

print(golomb_decode("11010", 4))