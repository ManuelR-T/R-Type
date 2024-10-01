# Sprint Meeting Summary

## Completed Tasks
- **Lobby Development:**
  A key milestone achieved in this sprint is the development of the Lobby system, which allows players to initiate games in separate instances.
  The server is responsible for handling client connections and requests, while the client side focuses on rendering the server's responses by making API calls to it.

## Discussion During the Meeting
- **Code Case Conventions:**
  We have established clear guidelines for naming conventions throughout the codebase.
  Moving forward, the team will consistently apply camelCase, PascalCase, snake_case, etc., based on the context.
- **Server-Client Architecture:**
  We also designed the new architecture for both the server and the client.
  This architecture will ensure better scalability and maintainability, laying the groundwork for future development.
  ![image](https://github.com/user-attachments/assets/b14ea274-b964-4113-946f-c7c2f16887c0)

- **Planning Future Tasks:**
  We outlined and defined several upcoming tasks that align with the overall project goals.

## Task Distribution
- **Manuel:**
  Currently focused on the continuous integration (CI) system for Windows, including the package manager and overall CI setup.
  He is also working on setting up a dual-boot environment.

- **Nicolas:**
  Nicolas is leading the UI design of the game using Figma and will soon transition to implementing the frontend using ImGui.

- **Thibaud:**
  Thibaud is enhancing the TCP protocol, focusing on optimizing and improving its performance.

- **Rahul:**
  Rahul is working on the ECS (Entity Component System) Factory, specifically the construction of the ECS architecture.
