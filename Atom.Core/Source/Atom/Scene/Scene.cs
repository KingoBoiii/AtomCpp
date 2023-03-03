namespace Atom
{

    public static class Scene
    {
        
        public static Entity CreateEntity(string name)
        {
            if(string.IsNullOrWhiteSpace(name))
            {
                Log.Error("Failed to create entity with empty name.");
                return null;
            }

            InternalCalls.Scene_CreateEntity(name, out var entityId);
            return new Entity(entityId);
        }

        public static void DestroyEntity(Entity entity)
        {
            if(entity == null)
            {
                return;
            }

            if(!IsEntityValid(entity))
            {
                return;
            }

            ulong entityId = entity.Id;
            InternalCalls.Scene_DestroyEntity(entityId);
        }

        public static Entity FindEntityByName(string name)
        {
            InternalCalls.Scene_FindEntityByName(name, out var entityId);
            return new Entity(entityId);
        }

        public static bool IsEntityValid(Entity entity)
        {
            return InternalCalls.Scene_IsEntityValid(entity.Id);
        }
        
    }

}
