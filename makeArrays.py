
import csv

def main():
    csvFile = open('Frequencies.csv', 'r')
    reader = csv.reader(csvFile)
    notes = 'Notes = {'
    frequencies = 'Frequencies = {'
    for line in reader:
        notes = notes + '"' + line[0] + '"' + ', '
        frequencies = frequencies + line[1] + ', '
    notes = notes[:-2] + '}'
    frequencies = frequencies[:-2] + '}'
    print(notes)
    print(frequencies)

if __name__ == '__main__':
    main()
