#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Define the structure to hold patient information
struct Patient {
 char patient_id[20];
 char name[100];
 int age;
 char diagnosis[100];
13
 char treatment[100];
};
// Function prototypes
void admitPatient(FILE *file);
void dischargePatient(FILE *file);
void viewAllPatients(FILE *file);
void searchPatientById(FILE *file);
void savePatientToFile(FILE *file, struct Patient *patient);
int fileExists(const char *filename);
int main() {
 // File to store patient records
 const char *fileName = "patients.txt";
 // Check if the file exists, if not, create it
 if (!fileExists(fileName)) {
 FILE *file = fopen(fileName, "w");
 if (file == NULL) {
 printf("Error opening file for writing.\n");
 return 1;
 }
 fclose(file);
 }
 int choice;
 FILE *file;
 while (1) {
 printf("\n--- Hospital Patient Management System ---\n");
 printf("1. Admit a new patient\n");
 printf("2. Discharge a patient\n");
 printf("3. View all patients\n");
 printf("4. Search for a patient by ID\n");
 printf("5. Exit\n");
 printf("Enter your choice (1-5): ");
 scanf("%d", &choice);
 getchar(); // To consume the newline character left by scanf
 switch (choice) {
 case 1:
 file = fopen(fileName, "a+"); // Open file in append mode
 admitPatient(file);
 fclose(file);
 break;
 case 2:
 file = fopen(fileName, "r+"); // Open file in read-write mode
 dischargePatient(file);
 fclose(file);
 break;
 case 3:
 file = fopen(fileName, "r"); // Open file in read mode
 viewAllPatients(file);
14
 fclose(file);
 break;
 case 4:
 file = fopen(fileName, "r"); // Open file in read mode
 searchPatientById(file);
 fclose(file);
 break;
 case 5:
 printf("Exiting the system.\n");
 return 0;
 default:
 printf("Invalid choice. Please select a valid option.\n");
 }
 }
 return 0;
}
// Function to admit a new patient
void admitPatient(FILE *file) {
 struct Patient newPatient;
 printf("Enter patient ID: ");
 fgets(newPatient.patient_id, sizeof(newPatient.patient_id), stdin);
 newPatient.patient_id[strcspn(newPatient.patient_id, "\n")] = 0; // Remove the newline 
character
 printf("Enter patient name: ");
 fgets(newPatient.name, sizeof(newPatient.name), stdin);
 newPatient.name[strcspn(newPatient.name, "\n")] = 0; // Remove the newline character
 printf("Enter patient age: ");
 scanf("%d", &newPatient.age);
 getchar(); // To consume the newline character left by scanf
 printf("Enter diagnosis: ");
 fgets(newPatient.diagnosis, sizeof(newPatient.diagnosis), stdin);
 newPatient.diagnosis[strcspn(newPatient.diagnosis, "\n")] = 0; // Remove the newline 
character
 printf("Enter treatment: ");
 fgets(newPatient.treatment, sizeof(newPatient.treatment), stdin);
 newPatient.treatment[strcspn(newPatient.treatment, "\n")] = 0; // Remove the newline 
character
 savePatientToFile(file, &newPatient);
 printf("Patient admitted successfully.\n");
}
// Function to save a patient record to file
void savePatientToFile(FILE *file, struct Patient *patient) {
 fprintf(file, "%s,%s,%d,%s,%s\n", patient->patient_id, patient->name, patient->age, 
15
patient->diagnosis, patient->treatment);
}
// Function to discharge a patient
void dischargePatient(FILE *file) {
 char patientId[20];
 printf("Enter patient ID to discharge: ");
 fgets(patientId, sizeof(patientId), stdin);
 patientId[strcspn(patientId, "\n")] = 0; // Remove the newline character
 FILE *tempFile = fopen("temp.txt", "w"); // Temporary file to store data after removal
 struct Patient patient;
 int found = 0;
 while (fscanf(file, "%19[^,],%99[^,],%d,%99[^,],%99[^\n]\n", patient.patient_id, 
patient.name, &patient.age, patient.diagnosis, patient.treatment) == 5) {
 if (strcmp(patient.patient_id, patientId) != 0) {
 savePatientToFile(tempFile, &patient);
 } else {
 found = 1;
 }
 }
 fclose(file);
 fclose(tempFile);
 if (found) {
 remove("patients.txt"); // Remove the original file
 rename("temp.txt", "patients.txt"); // Rename the temporary file to the original file 
name
 printf("Patient with ID %s discharged successfully.\n", patientId);
 } else {
 printf("Patient with ID %s not found.\n", patientId);
 }
}
// Function to view all patients
void viewAllPatients(FILE *file) {
 struct Patient patient;
 int found = 0;
 printf("\nList of all patients:\n");
 while (fscanf(file, "%19[^,],%99[^,],%d,%99[^,],%99[^\n]\n", patient.patient_id, 
patient.name, &patient.age, patient.diagnosis, patient.treatment) == 5) {
 printf("ID: %s, Name: %s, Age: %d, Diagnosis: %s, Treatment: %s\n",
 patient.patient_id, patient.name, patient.age, patient.diagnosis, patient.treatment);
 found = 1;
 }
 if (!found) {
 printf("No patients found.\n");
 }
16
}
// Function to search for a patient by ID
void searchPatientById(FILE *file) {
 char patientId[20];
 printf("Enter patient ID to search: ");
 fgets(patientId, sizeof(patientId), stdin);
 patientId[strcspn(patientId, "\n")] = 0; // Remove the newline character
 struct Patient patient;
 int found = 0;
 while (fscanf(file, "%19[^,],%99[^,],%d,%99[^,],%99[^\n]\n", patient.patient_id, 
patient.name, &patient.age, patient.diagnosis, patient.treatment) == 5) {
 if (strcmp(patient.patient_id, patientId) == 0) {
 printf("\nPatient found:\n");
 printf("ID: %s, Name: %s, Age: %d, Diagnosis: %s, Treatment: %s\n",
 patient.patient_id, patient.name, patient.age, patient.diagnosis, patient.treatment);
 found = 1;
 break;
 }
 }
 if (!found) {
 printf("Patient with ID %s not found.\n", patientId);
 }
}
// Function to check if a file exists
int fileExists(const char *filename) {
 FILE *file = fopen(filename, "r");
 if (file != NULL) {
 fclose(file);
 return 1;
 }
 return 0;
}