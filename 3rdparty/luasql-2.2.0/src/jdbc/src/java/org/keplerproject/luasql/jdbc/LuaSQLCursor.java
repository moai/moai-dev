package org.keplerproject.luasql.jdbc;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Types;

import org.keplerproject.luajava.JavaFunction;
import org.keplerproject.luajava.LuaException;
import org.keplerproject.luajava.LuaObject;
import org.keplerproject.luajava.LuaState;

/**
 * LuaSQL JDBC cursor. Is implemented in Java because it's more efficient.
 * The fetch method was slow in Lua, due to many accesses to Java.
 * 
 * @author Thiago Ponte
 */
public class LuaSQLCursor
{
  private LuaState  L;
  private ResultSet rs;
  
  /**
   * Function that open the Lib.
   */
  static public int open(LuaState L) throws LuaException
  {
    L.pushJavaFunction(new JavaFunction(L){

      /**
       * Creates a LuaSQLCursor and returns it.
       */
      public int execute() throws LuaException
      {
        ResultSet rs = (ResultSet) L.getObjectFromUserdata(2);
        
        L.pushJavaObject(new LuaSQLCursor(L, rs));
        
        return 1;
      }
    });
    
    return 1;
  }
  
  protected LuaSQLCursor(LuaState L, ResultSet rs)
  {
    this.L         = L;
    this.rs        = rs;
  }

  /**
   * 
   * @param table the results will be copied into the table and this table will be returned.
   * @param modeString the results will be copied into the table and this table will be returned<br>
   * "n" the resulting table will have numerical indices (default)<br>
   * "a" the resulting table will have alphanumerical indices  
   * @return the parameter <code>table</code>
   */
  public LuaObject fetch(LuaObject table, String modeString)
  {
    try
    {
      if (!rs.next())
        return null;
      
      table.push();

      ResultSetMetaData md = rs.getMetaData();
      int columnCount = md.getColumnCount();
      for (int i = 1; i <= columnCount; i++)
      {
        int type = md.getColumnType(i);
        
/*        if ("a".equalsIgnoreCase(modeString))
          L.pushString(md.getColumnName(i));
        else
          L.pushNumber(i);*/
        
        switch (type)
        {
          case Types.INTEGER: case Types.BIGINT: case Types.SMALLINT:
          case Types.DECIMAL: case Types.DOUBLE: case Types.FLOAT:
          case Types.NUMERIC: case Types.REAL: case Types.TINYINT:
            
            L.pushNumber(rs.getDouble(i));
            break;
            
          case Types.CHAR: case Types.VARCHAR:
          case Types.LONGVARCHAR: case Types.CLOB:

            L.pushString(rs.getString(i));
            break;

          case Types.BINARY: case Types.VARBINARY:
          case Types.LONGVARBINARY: case Types.BLOB:
            
            L.pushString(rs.getBytes(i));
            break;
          
          case Types.BIT: case Types.BOOLEAN:
            
            L.pushBoolean(rs.getBoolean(i)? 1 : 0);
            break;
          
          case Types.DATE: case Types.TIME: case Types.TIMESTAMP:
            
            L.pushString(rs.getDate(i).toString());
            break;
            
          case Types.NULL:
            
            L.pushNil();
            break;
          
          default:
            
            L.pushString(rs.getString(i));
            break;
        }
        
        if (modeString.contains("a"))
        {
          L.pushString(md.getColumnName(i));
          L.pushValue(-2);
          L.setTable(-4);
        }
        if (modeString.contains("n"))
        {
          L.pushNumber(i);
          L.pushValue(-2);
          L.setTable(-4);
        }
        
        L.pop(1);
      }
      
      L.pop(1);

      return table;
    }
    catch (SQLException e)
    {
      return table;
    }
  }
  /**
   * Gets the name of the columns.
   * 
   * @return a list of column names.
   */
  public LuaObject getcolnames() throws SQLException
  {
    L.newTable();
    LuaObject table = L.getLuaObject(-1);

    ResultSetMetaData md = rs.getMetaData();

    for (int i = 1; i <= md.getColumnCount(); i++)
    {
      String name = md.getColumnName(i);

      L.pushNumber(i);
      L.pushString(name);
      L.setTable(-3);
    }
    L.pop(1);

    return table;
  }

  /**
   * Gets the types of the columns
   * 
   * @return a list of column types.
   */
  public LuaObject getcoltypes() throws SQLException
  {
    L.newTable();
    LuaObject table = L.getLuaObject(-1);

    ResultSetMetaData md = rs.getMetaData();

    for (int i = 1; i <= md.getColumnCount(); i++)
    {
      String name = md.getColumnTypeName(i);

      L.pushNumber(i);
      L.pushString(name);
      L.setTable(-3);
    }
    L.pop(1);

    return table;
  }
}
