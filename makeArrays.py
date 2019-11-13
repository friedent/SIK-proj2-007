
import csv

def main():
    csvFile = open('FrequenciesSmall.csv', 'r')
    reader = csv.reader(csvFile)
    i = 0
    notes = 'Notes = {'
    frequencies = 'Frequencies = {'
    for line in reader:
        i=i+1
        notes = notes + '"' + line[0] + '"' + ', '
        frequencies = frequencies + line[1] + ', '
    notes = notes[:-2] + '}'
    frequencies = frequencies[:-2] + '}'
    print(notes)
    print(frequencies)
    print(i)

if __name__ == '__main__':
    main()
