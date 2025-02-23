# 🔫 Modular Weapon System

A flexible and modular weapon system for game development, designed for easy integration and customization.

![License](https://img.shields.io/badge/license-MIT-blue.svg) ![Version](https://img.shields.io/badge/version-1.0.0-green.svg)

## 📋 Quick Start Guide

This guide provides step-by-step instructions to set up the modular weapon system.

---

### 🔧 **Installation and Setup**

1. **Import your weapons** into the project.  
2. **Create a blueprint** based on the `Weapon` class.  
3. **Assign a Skeletal Mesh** to the appropriate component.  
4. **Add at least one component** based on `WeaponPartAttachmentPoint`:  
   - Configure the desired values in its properties.  
   - Position it correctly in the blueprint.  
5. **Add weapons** to the `DT_Weapons` data table.  
6. **Add attachments** to the `DT_Attachments` data table.  

---

### 🎮 **Controls**

- 🖱️ `LMB (Hold)` – Rotate object  
- 🔍 `Scroll` – Zoom in/out the camera  

Once these steps are complete, the system is ready for use!

---

## 📖 Detailed Setup Guide

### 🔗 Importing Weapons

- **Base Weapon Mesh:**  
  - Use a **Skeletal Mesh**  
  - Rigging is not required but should be enabled during import  

- **Attachments:**  
  - Use **Static Meshes**  
  - Disable Skeletal Mesh settings during import  

**Recommendation:** Organize all weapons and related assets in a dedicated `Weapons` folder.

---

### 🏗️ Creating a Weapon Blueprint

1. Create a blueprint based on the `Weapon` class.  
2. Assign the weapon model to the `WeaponMesh` component.  

---

### ➕ Adding Attachment Points

**Key Properties:**

- `WeaponPartType`: Specifies the category of parts that can be attached  
- `BrushTexture`: Texture used to display the slot  
- `UsableTable`: Data table listing available categories for the slot  
- `InitialWeaponPartRow`: Default initial values for the slot  
- `StaticMeshComponent`: Leave empty or use for previewing parts in the editor  

---

### 📊 Adding Weapons to the Data Table

1. Open the `DT_Weapons` table.  
2. Click the **plus (+)** button to add a new entry.  
3. Set the following properties:  
   - `Name`: Unique identifier (not used directly)  
   - `Weapon`: Blueprint containing the configured weapon  
   - `LinkedInventoryItemSlotWidget`: (Optional) Reference a widget blueprint for slots  

---

### ⚙️ Adding Attachments to the Data Table

1. Open the `DT_Attachments` table.  
2. Configure the following properties:  
   - `Name`: Display name for the attachment  
   - `BaseWeaponPartData`: Category of the attachment  
   - `StaticMesh`: Assign the Static Mesh  
   - `Texture`: Texture used for the slot material  

---

## 🛠️ System Settings

Settings can be found in the `Player Controller` class (`BP_PC_MW` blueprint).

### 🔄 Rotation Settings

- `CanRotate`: Enable/disable rotation  
- `RotationSpeed`: Speed of rotation  
- `XAxisMinClamp` / `XAxisMaxClamp`: Yaw rotation range (left-right)  
- `YAxisMinClamp` / `YAxisMaxClamp`: Pitch rotation range (up-down)  

### 🎥 Camera Settings

- `InitialRelativeLocation`: Initial camera position  
- `ReverseWeapon`: Set weapon direction  
- `CameraProjectionMode`: Projection type (perspective, orthographic)  
- `FOVAngle`: Field of view  
- `OrthoWidth`: Width for orthographic mode  
- `CenterWeapon`: Center weapon in the camera view  
- `CameraOffsetSpeed`: Speed of camera movement  
- `MinOffsetDistance`: Minimum offset distance  
- `MaxOffsetDistance`: Maximum offset distance  

### 🖼️ UI Settings

- `MaxItemWidgets`: Max number of item widgets  
- `WidgetsMethodLocation`: Layout method for widgets  
- `InventoryItemSlotWidget`: Widget class reference for slots  
- `BaseRenderTargetMaterial`: Base material for render target  
- `ItemPartWidgetClass`: Widget class for item parts  
- `IconMaterial`: Material for item icons  

### 📂 Additional Settings

- `WeaponTable`: Reference weapon data table  
- `InventorySceneRendererClass`: Renderer class for inventory scenes  
- `InventoryItemSlotWidgetClass`: Widget class for inventory item slots  

---

## 🚀 Getting Started

After completing the steps above, your modular weapon system should be fully functional and ready for use!

---

## 💡 Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss what you would like to change.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
