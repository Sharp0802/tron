using System.Collections.Concurrent;
using System.Linq.Expressions;

namespace Tron.Runtime.Reflection;

internal static class Dynamic
{
    private record ConstructorKey(Type Type, Type[] Args);
    
    public delegate object ConstructorDelegate(params object[] args);

    private static readonly ConcurrentDictionary<ConstructorKey, ConstructorDelegate> Cache = new();
    
    public static ConstructorDelegate GetCtor(Type cls, params Type[] arguments)
    {
        return Cache.GetOrAdd(new ConstructorKey(cls, arguments), static key =>
        {
            var ctor = key.Type.GetConstructor(key.Args);
            if (ctor is null)
                throw new MissingMethodException(key.Type.FullName, ".ctor");

            var param = Expression.Parameter(typeof(object[]));

            var body = Expression.New(
                ctor,
                key.Args
                   .Select((type, i) =>
                       Expression.Convert(
                           Expression.ArrayAccess(param, Expression.Constant(i)),
                           type))
                   .Select(exp => (Expression)exp)
                   .ToArray());

            return Expression.Lambda<ConstructorDelegate>(body, param).Compile();
        });
    }
}