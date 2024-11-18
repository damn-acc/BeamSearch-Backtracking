# Ініціалізація списку суміжностей для всіх 47 префектур
adjacency_list = {
    1: [2],               # Hokkaido
    2: [1, 3, 5],         # Aomori
    3: [2, 4, 5, 6],      # Iwate
    4: [3, 5, 6, 7],         # Miyagi
    5: [2, 3, 4, 6],      # Akita
    6: [3, 5, 4, 7, 15], # Yamagata
    7: [4, 6, 9, 8, 10, 15],     # Fukushima
    8: [7, 9, 10, 11, 12],     # Niigata
    9: [15, 7, 10, 8],  # Gunma
    10: [9, 15 ,20, 11, 7, 8],    # Tochigi
    11: [10, 8, 20, 19, 13, 12],        # Nagano
    12: [8, 11, 13, 14],      # Ibaraki
    13: [11, 12, 19, 14],              # Chiba
    14: [19, 13, 12, 22],              # Tokyo
    15: [6, 7, 9, 10, 20, 16],              # Kanagawa
    16: [15, 20, 21, 17],          # Saitama
    17: [16, 21, 18],              # Shizuoka
    18: [17, 21, 25, 26],          # Yamanashi
    19: [20, 11, 13, 14, 22],       # Gifu
    20: [15, 10, 11, 19, 22, 23, 21, 16],  # Aichi
    21: [17, 16, 20, 23, 24, 25, 18],      # Shiga
    22: [23, 20, 19, 14],      # Kyoto
    23: [22, 20, 21, 24],      # Osaka
    24: [23, 21, 25, 26, 29, 30],      # Hyogo
    25: [24, 21, 18, 26],          # Nara
    26: [25, 24, 18, 28, 27, 29],      # Wakayama
    27: [26, 28, 29, 30],      # Tottori
    28: [27, 26, 31, 33],      # Shimane
    29: [27, 26, 24, 30],      # Okayama
    30: [29, 27, 24],      # Hiroshima
    31: [28, 33, 34, 32],      # Yamaguchi
    32: [31, 34, 35],      # Tokushima
    33: [31, 28, 32, 34],      # Kagawa
    34: [33, 31, 32, 35],      # Ehime
    35: [32, 34, 40],      # Kochi
    36: [37, 38, 39, 30],      # Fukuoka
    37: [36, 38],      # Saga
    38: [37, 36, 39, 44],      # Nagasaki
    39: [36, 38],      # Kumamoto
    40: [35, 41, 43, 44],      # Oita
    41: [40, 43, 42],      # Miyazaki
    42: [41, 43, 46],      # Kagoshima
    43: [41, 42, 40, 44, 45, 46],      # Okinawa
    44: [40, 43, 45, 38],              # Okinawa islands
    45: [46, 43, 44],                  # Amami islands
    46: [43, 45, 42, 47],              # Southernmost islands
    47: [46]                   # Okinawa Main
}

# Створення матриці суміжності
NUM_PREFECTURES = 47
adjacency_matrix = [[0] * NUM_PREFECTURES for _ in range(NUM_PREFECTURES)]

for prefecture, neighbors in adjacency_list.items():
    for neighbor in neighbors:
        adjacency_matrix[prefecture - 1][neighbor - 1] = 1
        adjacency_matrix[neighbor - 1][prefecture - 1] = 1  # Симетрія

# Вивід матриці
for i in range(NUM_PREFECTURES):
    for j in range(NUM_PREFECTURES):
        if (j == NUM_PREFECTURES - 1):
            print(adjacency_matrix[i][j])
        else:
            print(adjacency_matrix[i][j], end = ',')
