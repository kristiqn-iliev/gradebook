# Gradebook Console Application

This is a C++ CLI application for managing a gradebook system with Students, Subjects, and Grades, built using Visual Studio and the SOCI library via ODBC.

---

## 💻 Features

- List/Add/Edit/Delete Students
- List/Add/Edit/Delete Subjects
- Add/Edit/Delete Grades
- Query student performance across subjects

---

## ✅ Requirements

- **Visual Studio 2019 or later**
- **vcpkg**
- **ODBC Driver 17 for SQL Server**
- **MS SQL Server (Local or Remote)**

---

## 📦 Dependencies

All dependencies are managed via [vcpkg](https://github.com/microsoft/vcpkg).

### Required Libraries:

- [SOCI](https://github.com/SOCI/soci) (ODBC backend)

### Install vcpkg and Required Packages

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install soci[odbc]
```
## Setup

1. Run `create_database.sql` to create the database.
2. Run `init_schemas.sql` to initialize schema.
3. Run `seed_data.sql` to insert test data.
4. Update `.env` with your DB credentials.

### Building the Project (Visual Studio)

1. Open the solution/project in Visual Studio.
2. Make sure to set:
   - Platform: `x64`
   - Configuration: `Release` or `Debug`
3. Set the include and library paths to vcpkg manually:
   - Right-click project → Properties → VC++ Directories
     - Include Directories: `path\to\vcpkg\installed\x64-windows\include`
     - Library Directories: `path\to\vcpkg\installed\x64-windows\lib`

   - Also, in Linker → Input → Additional Dependencies, add:
     ```
     soci_core_4_0.lib
     soci_odbc_4_0.lib
     ```

4. Build and run.