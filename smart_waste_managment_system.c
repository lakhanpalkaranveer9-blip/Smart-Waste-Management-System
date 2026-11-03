#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BINS 100
#define DATA_FILE "waste_data.txt"
#define HISTORY_FILE "waste_history.txt"

typedef struct {
    int id;
    char location[50];
    char type[20];
    float capacity;
    float fill;
    int collections;
    int maintenance;
    int active;
} WasteBin;

WasteBin bins[MAX_BINS];
int binCount = 0;

float totalGenerated = 0;
float totalCollected = 0;

/* ---------- FILE HANDLING ---------- */

void saveData(void) {
    FILE *fp = fopen(DATA_FILE, "w");

    if (!fp) {
        printf("Error saving data!\\n");
        return;
    }

    fprintf(fp, "%d %.2f %.2f\\n",
            binCount, totalGenerated, totalCollected);

    for (int i = 0; i < binCount; i++) {
        fprintf(fp, "%d %s %s %.2f %.2f %d %d %d\\n",
                bins[i].id,
                bins[i].location,
                bins[i].type,
                bins[i].capacity,
                bins[i].fill,
                bins[i].collections,
                bins[i].maintenance,
                bins[i].active);
    }

    fclose(fp);
}

void loadData(void) {
    FILE *fp = fopen(DATA_FILE, "r");

    if (!fp)
        return;

    if (fscanf(fp, "%d %f %f",
               &binCount, &totalGenerated, &totalCollected) != 3) {
        binCount = 0;
        totalGenerated = 0;
        totalCollected = 0;
        fclose(fp);
        return;
    }

    if (binCount < 0)
        binCount = 0;
    if (binCount > MAX_BINS)
        binCount = MAX_BINS;

    for (int i = 0; i < binCount; i++) {
        if (fscanf(fp, "%d %49s %19s %f %f %d %d %d",
                   &bins[i].id,
                   bins[i].location,
                   bins[i].type,
                   &bins[i].capacity,
                   &bins[i].fill,
                   &bins[i].collections,
                   &bins[i].maintenance,
                   &bins[i].active) != 8) {
            binCount = i;
            break;
        }
    }

    fclose(fp);
}

void logCollection(int id, float amount, const char type[]) {
    FILE *fp = fopen(HISTORY_FILE, "a");

    if (!fp)
        return;

    time_t now = time(NULL);

    fprintf(fp, "%lld %d %.2f %s\\n",
            (long long)now, id, amount, type);

    fclose(fp);
}

/* ---------- BIN MANAGEMENT ---------- */

void addBin(void) {
    if (binCount >= MAX_BINS) {
        printf("Maximum bin limit reached!\\n");
        return;
    }

    WasteBin *b = &bins[binCount];
    b->id = binCount + 1;

    printf("Enter location (single word, e.g. Library): ");
    if (scanf("%49s", b->location) != 1)
        return;

    printf("Enter capacity in kg: ");
    if (scanf("%f", &b->capacity) != 1 || b->capacity <= 0) {
        printf("Invalid capacity!\\n");
        return;
    }

    printf("\\nWaste category:\\n");
    printf("1. Biodegradable\\n");
    printf("2. Recyclable\\n");
    printf("3. Hazardous\\n");
    printf("4. General\\n");
    printf("Choice: ");

    int choice;
    if (scanf("%d", &choice) != 1)
        return;

    switch (choice) {
        case 1: strcpy(b->type, "Bio"); break;
        case 2: strcpy(b->type, "Recycle"); break;
        case 3: strcpy(b->type, "Hazardous"); break;
        default: strcpy(b->type, "General"); break;
    }

    b->fill = 0;
    b->collections = 0;
    b->maintenance = 0;
    b->active = 1;

    binCount++;
    saveData();

    printf("Bin registered successfully! ID: %d\\n", b->id);
}

void addWaste(void) {
    int id;
    float amount;

    printf("Enter bin ID: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Invalid bin!\\n");
        return;
    }

    WasteBin *b = &bins[id - 1];

    if (b->maintenance) {
        printf("Bin is under maintenance!\\n");
        return;
    }

    printf("Enter waste amount (kg): ");
    if (scanf("%f", &amount) != 1)
        return;

    if (amount <= 0 || b->fill + amount > b->capacity) {
        printf("Invalid amount or bin capacity exceeded!\\n");
        return;
    }

    b->fill += amount;
    totalGenerated += amount;

    saveData();
    printf("Waste added successfully!\\n");
}

void editBin(void) {
    int id;

    printf("Enter bin ID to edit: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Invalid bin!\\n");
        return;
    }

    WasteBin *b = &bins[id - 1];

    printf("New location (single word): ");
    if (scanf("%49s", b->location) != 1)
        return;

    printf("New capacity: ");
    if (scanf("%f", &b->capacity) != 1 ||
        b->capacity < b->fill || b->capacity <= 0) {
        printf("Capacity cannot be below current waste and must be positive!\\n");
        return;
    }

    saveData();
    printf("Bin updated successfully!\\n");
}

void deleteBin(void) {
    int id;

    printf("Enter bin ID to delete: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Invalid bin!\\n");
        return;
    }

    bins[id - 1].active = 0;
    saveData();

    printf("Bin removed successfully!\\n");
}

/* ---------- MAINTENANCE ---------- */

void maintenanceMenu(void) {
    int id, choice;

    printf("Enter bin ID: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Invalid bin!\\n");
        return;
    }

    printf("1. Mark for maintenance\\n");
    printf("2. Complete maintenance\\n");
    printf("Choice: ");
    if (scanf("%d", &choice) != 1)
        return;

    if (choice == 1)
        bins[id - 1].maintenance = 1;
    else if (choice == 2)
        bins[id - 1].maintenance = 0;
    else {
        printf("Invalid choice!\\n");
        return;
    }

    saveData();
    printf("Maintenance status updated!\\n");
}

/* ---------- COLLECTION ---------- */

void collectBin(int id) {
    WasteBin *b = &bins[id - 1];

    if (b->fill <= 0) {
        printf("Bin %d is empty.\\n", id);
        return;
    }

    float amount = b->fill;

    printf("Collected %.2f kg from bin %d (%s)\\n",
           amount, b->id, b->location);

    totalCollected += amount;
    logCollection(b->id, amount, b->type);

    b->fill = 0;
    b->collections++;

    saveData();
}

void collectWaste(void) {
    int id;

    printf("Enter bin ID: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Invalid bin!\\n");
        return;
    }

    if (bins[id - 1].maintenance) {
        printf("Bin is under maintenance!\\n");
        return;
    }

    collectBin(id);
}

void collectAll(void) {
    int collected = 0;

    for (int i = 0; i < binCount; i++) {
        if (bins[i].active && !bins[i].maintenance && bins[i].fill > 0) {
            collectBin(i + 1);
            collected++;
        }
    }

    if (collected == 0)
        printf("No eligible bins contain waste.\\n");
}

/* ---------- SMART PRIORITY / ROUTE ---------- */

void smartPriority(void) {
    int order[MAX_BINS];
    int count = 0;

    for (int i = 0; i < binCount; i++) {
        if (bins[i].active && !bins[i].maintenance && bins[i].fill > 0)
            order[count++] = i;
    }

    /* Sort bins by highest fill percentage */
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            float a = bins[order[i]].fill / bins[order[i]].capacity;
            float b = bins[order[j]].fill / bins[order[j]].capacity;

            if (b > a) {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    printf("\\nSMART COLLECTION PRIORITY\\n");

    for (int i = 0; i < count; i++) {
        WasteBin b = bins[order[i]];

        printf("%d. Bin %d - %s (%.1f%% full)\\n",
               i + 1, b.id, b.location,
               (b.fill / b.capacity) * 100);
    }

    if (count == 0)
        printf("No bins need collection.\\n");
}

void routePlanner(void) {
    printf("\\n========== COLLECTION ROUTE ==========");
    smartPriority();
    printf("\\nCollect bins in the priority order above.\\n");
    printf("Route is based on fill percentage, not geographic distance.\\n");
}

/* ---------- SEARCH ---------- */

void searchBin(void) {
    int id;

    printf("Enter bin ID to search: ");
    if (scanf("%d", &id) != 1)
        return;

    if (id < 1 || id > binCount || !bins[id - 1].active) {
        printf("Bin not found!\\n");
        return;
    }

    WasteBin b = bins[id - 1];

    printf("\\nBin ID: %d\\n", b.id);
    printf("Location: %s\\n", b.location);
    printf("Category: %s\\n", b.type);
    printf("Capacity: %.2f kg\\n", b.capacity);
    printf("Waste: %.2f kg\\n", b.fill);
    printf("Collections: %d\\n", b.collections);
    printf("Maintenance: %s\\n",
           b.maintenance ? "Required" : "Normal");
}

/* ---------- DASHBOARD ---------- */

void dashboard(void) {
    printf("\\n========== WASTE DASHBOARD ==========\\n");

    float totalCapacity = 0;
    float currentWaste = 0;

    for (int i = 0; i < binCount; i++) {
        if (!bins[i].active)
            continue;

        WasteBin b = bins[i];

        totalCapacity += b.capacity;
        currentWaste += b.fill;

        float percent = b.fill / b.capacity * 100;

        printf("\\nBin %d - %s [%s]\\n",
               b.id, b.location, b.type);

        printf("[");
        int bars = (int)(percent / 5);

        if (bars > 20)
            bars = 20;

        for (int j = 0; j < 20; j++)
            printf("%s", j < bars ? "#" : "-");

        printf("] %.1f%%\\n", percent);

        if (b.maintenance)
            printf("STATUS: MAINTENANCE\\n");
        else if (percent >= 80)
            printf("STATUS: WARNING\\n");
        else
            printf("STATUS: NORMAL\\n");
    }

    printf("\\nTotal capacity: %.2f kg\\n", totalCapacity);
    printf("Current waste: %.2f kg\\n", currentWaste);
    printf("Total generated: %.2f kg\\n", totalGenerated);
    printf("Total collected: %.2f kg\\n", totalCollected);
    printf("=====================================\\n");
}

/* ---------- HISTORY / STATISTICS ---------- */

void statistics(void) {
    FILE *fp = fopen(HISTORY_FILE, "r");

    if (!fp) {
        printf("No collection history available.\\n");
        return;
    }

    time_t now = time(NULL);

    float daily = 0, weekly = 0, lifetime = 0;
    float recyclable = 0;

    long long timestamp;
    int id;
    float amount;
    char type[20];

    while (fscanf(fp, "%lld %d %f %19s",
                  &timestamp, &id, &amount, type) == 4) {
        lifetime += amount;

        double age = difftime(now, (time_t)timestamp);

        if (age >= 0 && age <= 86400)
            daily += amount;

        if (age >= 0 && age <= 604800)
            weekly += amount;

        if (strcmp(type, "Recycle") == 0)
            recyclable += amount;
    }

    fclose(fp);

    printf("\\n========== COLLECTION STATISTICS ==========\\n");
    printf("Collected today: %.2f kg\\n", daily);
    printf("Collected this week: %.2f kg\\n", weekly);
    printf("Total recorded collection: %.2f kg\\n", lifetime);
    printf("Recyclable waste collected: %.2f kg\\n", recyclable);

    if (lifetime > 0)
        printf("Recycling percentage: %.2f%%\\n",
               recyclable / lifetime * 100);

    printf("===========================================\\n");
}

void collectionHistory(void) {
    FILE *fp = fopen(HISTORY_FILE, "r");

    if (!fp) {
        printf("No collection history found.\\n");
        return;
    }

    long long timestamp;
    int id;
    float amount;
    char type[20];

    printf("\\n========== COLLECTION HISTORY ==========\\n");

    while (fscanf(fp, "%lld %d %f %19s",
                  &timestamp, &id, &amount, type) == 4) {
        time_t t = (time_t)timestamp;
        char *date = ctime(&t);

        if (date)
            printf("%s", date);

        printf("Bin %d | %.2f kg | %s\\n\\n",
               id, amount, type);
    }

    fclose(fp);
}

/* ---------- ENVIRONMENTAL IMPACT ---------- */

void environmentalImpact(void) {
    FILE *fp = fopen(HISTORY_FILE, "r");

    if (!fp) {
        printf("No collection data available.\\n");
        return;
    }

    float recyclable = 0;

    long long timestamp;
    int id;
    float amount;
    char type[20];

    while (fscanf(fp, "%lld %d %f %19s",
                  &timestamp, &id, &amount, type) == 4) {
        if (strcmp(type, "Recycle") == 0)
            recyclable += amount;
    }

    fclose(fp);

    printf("\\n========== ENVIRONMENTAL IMPACT ==========\\n");
    printf("Recyclable waste collected: %.2f kg\\n", recyclable);
    printf("Estimated landfill diversion: %.2f kg\\n", recyclable);

    /* Illustrative estimate, not a verified environmental measurement */
    printf("Estimated CO2 equivalent avoided: %.2f kg\\n",
           recyclable * 0.5);

    printf("\\nNote: CO2 estimate uses an illustrative factor\\n");
    printf("of 0.5 kg CO2e per kg recycled. Actual impact varies.\\n");
    printf("==========================================\\n");
}

/* ---------- MENU ---------- */

void menu(void) {
    printf("\\n====================================\\n");
    printf(" SMART WASTE MANAGEMENT SYSTEM\\n");
    printf("====================================\\n");
    printf("1. Add Waste Bin\\n");
    printf("2. Add Waste\\n");
    printf("3. View Dashboard\\n");
    printf("4. Collect Waste from Bin\\n");
    printf("5. Collect Waste from All Bins\\n");
    printf("6. Smart Collection Priority\\n");
    printf("7. Collection Route Planner\\n");
    printf("8. Collection History\\n");
    printf("9. Statistics & Recycling Report\\n");
    printf("10. Search Bin\\n");
    printf("11. Edit Bin\\n");
    printf("12. Delete Bin\\n");
    printf("13. Maintenance Management\\n");
    printf("14. Environmental Impact\\n");
    printf("15. Exit\\n");
    printf("====================================\\n");
    printf("Enter choice: ");
}

int main(void) {
    int choice;

    loadData();

    printf("\\nSMART WASTE MANAGEMENT SYSTEM\\n");
    printf("Existing data loaded (if available).\\n");

    do {
        menu();

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please restart and enter numbers only.\\n");
            return 1;
        }

        switch (choice) {
            case 1: addBin(); break;
            case 2: addWaste(); break;
            case 3: dashboard(); break;
            case 4: collectWaste(); break;
            case 5: collectAll(); break;
            case 6: smartPriority(); break;
            case 7: routePlanner(); break;
            case 8: collectionHistory(); break;
            case 9: statistics(); break;
            case 10: searchBin(); break;
            case 11: editBin(); break;
            case 12: deleteBin(); break;
            case 13: maintenanceMenu(); break;
            case 14: environmentalImpact(); break;

            case 15:
                saveData();
                printf("Data saved. Goodbye!\\n");
                break;

            default:
                printf("Invalid choice!\\n");
        }

    } while (choice != 15);

    return 0;
}
